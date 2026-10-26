#!/usr/bin/env python3
"""Extract production constructors; verify aligned, bounded geometry storage.
Real project types, host ASan/UBSan, no game/hardware execution.
"""
from pathlib import Path
import re
import subprocess
import sys
import tempfile

root = Path(__file__).resolve().parents[2]
source = (root / "src/carma2/common/physics.c").read_text()
names = ["AllocateShapeWireframePolyhedron", "AllocateShapePolyhedron", "FUN_00421fd0", "FUN_004222c0"]

def functions(text):
    result = []
    for name in names:
        match = re.search(r"^(?:tCollision_shape_\w+\*|int) C2_HOOK_FASTCALL " + name + r"\([^\n]*\) \{", text, re.M)
        assert match, name
        end = match.end()
        depth = 1
        while depth:
            depth += (text[end] == "{") - (text[end] == "}")
            end += 1
        result.append(text[match.start():end])
    return "\n".join(result)

prefix = r'''
#include "carpocalypse2_types.h"
#include "carpocalypse2_macros.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
static unsigned char* allocation;
static size_t capacity;
static tCollision_shape_polyhedron_data* captured;
static tPhysicsError C2_HOOK_FASTCALL FUN_00422400(tCollision_shape_polyhedron_data* p, int* indices, br_model* model, int count) {
    (void)indices; (void)model; (void)count;
    captured = p;
    p->count_points = 0;
    return 7;
}
void* BrMemAllocate(br_size_t size, br_uint_8 type) {
    (void)type;
    capacity = size;
    allocation = malloc(size + 16);
    assert(allocation);
    memset(allocation, 0xcc, size + 16);
    return allocation + 8;
}
'''
suffix = r'''
static void check(tCollision_shape_polyhedron_data* p, int points, int edges, int planes) {
    int i, j;
    assert(((uintptr_t)p->points % _Alignof(br_vector3)) == 0);
    assert(((uintptr_t)p->planes % _Alignof(br_vector4)) == 0);
    assert((unsigned char*)(p->points + points) <= (unsigned char*)p->edges);
    assert((unsigned char*)(p->edges + edges) <= (unsigned char*)p->planes);
    assert((unsigned char*)(p->planes + planes) <= allocation + 8 + capacity);
    for (i = 0; i < points; ++i) for (j = 0; j < 3; ++j) p->points[i].v[j] = i + j;
    for (i = 0; i < edges; ++i) p->edges[i].index1 = p->edges[i].index2 = i;
    for (i = 0; i < planes; ++i) for (j = 0; j < 4; ++j) p->planes[i].v[j] = i - j;
    for (i = 0; i < 8; ++i) assert(allocation[i] == 0xcc && allocation[capacity + 8 + i] == 0xcc);
    free(allocation);
}
int main(void) {
    int n;
    for (n = 0; n <= 255; ++n) {
        tCollision_shape_polyhedron* p = AllocateShapeWireframePolyhedron(n, 0);
        check(&p->polyhedron, n, n, n + 2);
    }
    for (n = 4; n <= 255; ++n) {
        tCollision_shape_polyhedron* p = AllocateShapePolyhedron(n, 0);
        check(&p->polyhedron, n, (n - 2) * 3, (n - 2) * 2);
        check(FUN_00421fd0(n), n, (n - 2) * 3, (n - 2) * 2);
        {
            br_model model = {0};
            int output_count, indices[256];
            model.nvertices = n;
            assert(FUN_004222c0(&output_count, indices, &model, 4, n) == 7);
            check(captured, n, (n - 2) * 3, (n - 2) * 2);
        }
    }
    puts("PASS all 4 production collision allocation paths: odd/even capacities, aligned points/planes, nonoverlapping arrays, full-capacity writes and guards; ASan/UBSan, real types, hull processing mocked, no game/hardware");
}
'''
with tempfile.TemporaryDirectory(prefix="c2-vita-collision-") as tmp:
    tmp = Path(tmp)
    c = tmp / "collision.c"
    includes = ["src/carma2/include", "src/carma2/hooks", "src/carma2/s3/include", "src/brender/include"]
    variants = [(False, source)]
    if len(sys.argv) == 2:
        variants.insert(0, (True, Path(sys.argv[1]).read_text()))
    for baseline, text in variants:
        # The old fixed-size 32-bit allocation loses its extra slack on a
        # 64-bit host. Reproduce its alignment fault using just n planes.
        test = suffix.replace("check(&p->polyhedron, n, n, n + 2);", "check(&p->polyhedron, n, n, n);") if baseline else suffix
        c.write_text(prefix + functions(text) + test)
        subprocess.run(["gcc", "-std=c11", "-O1", "-g", "-D__vita__", "-DCARPOCALYPSE2_FIX_BUGS",
                    "-fsanitize=address,undefined", "-fno-sanitize-recover=all",
                    *["-I" + str(root / p) for p in includes], str(c), "-o", str(tmp / "collision")], check=True)
        if baseline:
            run = subprocess.run([str(tmp / "collision")], capture_output=True, text=True)
            assert run.returncode != 0 and "p->planes % _Alignof(br_vector4)" in run.stderr, run.stderr
            print("PASS pre-fix constructors reproduce misaligned float-plane pointer", flush=True)
        else:
            subprocess.run([str(tmp / "collision")], check=True)
