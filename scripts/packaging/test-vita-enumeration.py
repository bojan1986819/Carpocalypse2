#!/usr/bin/env python3
"""Reproduce pre-fix Vita mount enumeration and test production directory code.
Actual case resolver + extracted production functions, mocked SDL filesystem
calls over real host files. No game/hardware execution.
"""
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

root=Path(__file__).resolve().parents[2]
sdk=Path(os.environ["VITASDK"])
current=(root/"src/carma2/platform/sdl3/sdl3.c").read_text()
git_dir=root/".git"
if git_dir.is_file():
    location=git_dir.read_text().split(":",1)[1].strip().replace("\\","/")
    if os.name != "nt" and re.match(r"^[A-Za-z]:/",location):
        location="/mnt/"+location[0].lower()+location[2:]
    git_dir=Path(location)
    if not git_dir.is_absolute():git_dir=root/git_dir
    common=git_dir/"commondir"
    if common.is_file():git_dir=(git_dir/common.read_text().strip()).resolve()
baseline=subprocess.check_output(["git","--git-dir",str(git_dir),"show","1cd8fed2:src/carma2/platform/sdl3/sdl3.c"],text=True)
def function(source,name):
    m=re.search(r"^(?:static int |static const char\* |int |void C2_HOOK_FASTCALL )"+name+r"\([^\n]*\) \{",source,re.M)
    assert m,name
    end=m.end();depth=1
    while depth:
        depth+=(source[end]=="{")-(source[end]=="}");end+=1
    return source[m.start():end]
prefix=r"""
#include <SDL3/SDL.h>
#include <assert.h>
#include <dirent.h>
#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#define C2_HOOK_FASTCALL
#define CARPOCALYPSE2_FIX_BUGS 1
#define SDL_strlen strlen
#define SDL_strcasecmp strcasecmp
#define SDL_free free
#define DRStricmp strcasecmp
#define dr_dprintf(...) ((void)0)
typedef char tPath_name[256];
typedef int (*tEnumPathCallback)(const char*,void*);
typedef void (*tPDForEveryFileRecurse_cbfn)(const char*);
typedef void (*tPDForEveryFile_cbfn)(const char*);
extern const char* carpocalypse2_fix_path_case(const char*);
size_t SDL_strlcpy(char* dst,const char* src,size_t size) {size_t n=strlen(src);if(size){size_t k=n<size-1?n:size-1;memcpy(dst,src,k);dst[k]=0;}return n;}
void PathCat(char* out,const char* dir,const char* name) {char tmp[256];assert(snprintf(tmp,sizeof(tmp),"%s/%s",dir,name)<sizeof(tmp));strcpy(out,tmp);}
char** SDL_GlobDirectory(const char* path,const char* pattern,SDL_GlobFlags flags,int* count) {
    DIR* dir=opendir(path[0]?path:".");struct dirent* entry;
    *count=0;if(!dir)return NULL;
    char** result=calloc(1,1024*sizeof(char*)+1024*256);assert(result);
    char* names=(char*)(result+1024);
    while((entry=readdir(dir)))if(strcmp(entry->d_name,".") && strcmp(entry->d_name,"..") && !fnmatch(pattern,entry->d_name,0)){
        assert(*count<1024);result[*count]=names+256*(*count);strcpy(result[*count],entry->d_name);(*count)++;
    }
    closedir(dir);return result;
}
bool SDL_GetPathInfo(const char* path,SDL_PathInfo* info) {
    struct stat st;if(stat(path,&st))return false;
    /* Match the SDK's short-enum byte store, leaving padding untouched. */
    unsigned char kind=S_ISREG(st.st_mode)?SDL_PATHTYPE_FILE:SDL_PATHTYPE_DIRECTORY;
    memcpy(&info->type,&kind,1);return true;
}
const char* SDL_GetError(void) {return "missing directory";}
static int hits;
static int collect(const char* path,void* data) {
    if(strstr(path,"EX00001.PIX")){FILE* f=fopen(path,"rb");assert(f);assert(fseek(f,0,SEEK_END)==0);assert(ftell(f)==8254);fclose(f);hits++;}
    return 0;
}
"""
suffix=r"""
#ifndef BASELINE
static int other(const char* path,void* data) {return 0;}
static int recursive_hits;
static void nested(const char* path) {
    FILE* f=fopen(path,"rb");assert(f);fclose(f);recursive_hits++;
    PDEnumPath("ux0:/data/c2port/DATA/other",other,NULL);
}
#endif
int main(void) {
    PDEnumPath("ux0:/data/c2port/DATA/COMMON/BOOM/PIX16",collect,NULL);
#ifdef BASELINE
    assert(hits==0);puts("PASS baseline reproduces missing ex00001: mount prefix lost");
#else
    assert(hits==1);hits=0;
    PDEnumPath("ux0:/data/c2port/DATA/COMMON/boom/PIX16",collect,NULL);assert(hits==1);
    hits=0;PDEnumPath("ux0:/data/c2port/DATA/COMMON/absent",collect,NULL);assert(!hits);
    PDForEveryFileRecurse("ux0:/data/c2port/DATA/COMMON/BOOM/PIX16",nested);assert(recursive_hits==2);
    recursive_hits=0;PDForEveryFile("ux0:/data/c2port/DATA/COMMON/BOOM/PIX16",nested);assert(recursive_hits==3);
    puts("PASS production enumeration resolves Vita mount + mixed-case BOOM, finds/opens real EX00001.PIX; missing directory safe, SDK byte-sized PathInfo type handled; mocked SDL, host files only");
#endif
}
"""
with tempfile.TemporaryDirectory(prefix="c2-vita-enum-") as tmp:
    tmp=Path(tmp);headers=tmp/"include";headers.mkdir();(headers/"SDL3").symlink_to(sdk/"arm-vita-eabi/include/SDL3",target_is_directory=True)
    assets=tmp/"ux0:/data/c2port/DATA/COMMON/boom/PIX16";assets.mkdir(parents=True)
    shutil.copyfile(__import__("pathlib").Path(__import__("os").environ["C2_GAME_DIR"])/"DATA/COMMON/boom/PIX16/EX00001.PIX",assets/"EX00001.PIX")
    otherdir=tmp/"ux0:/data/c2port/DATA/other";otherdir.mkdir(parents=True);(otherdir/"asset.pix").write_bytes(b"other")
    (assets/"ignore.lnk").write_text("skip");(assets/"directory.pix").mkdir()
    for name,source in [("baseline",baseline),("fixed",current)]:
        code=prefix
        if name=="baseline":code+="#define BASELINE 1\n"+function(source,"CI_ResolveDirCaseInsensitive")
        else:code+=function(source,"PDResolvePathCase")
        code+=function(source,"PDEnumPath")
        if name=="fixed":code+=function(source,"PDForEveryFileRecurse")+function(source,"PDForEveryFile")
        code+=suffix
        fixture=tmp/(name+".c");fixture.write_text(code)
        subprocess.run(["gcc","-std=c11","-O2","-I"+str(headers),str(fixture),str(root/"src/carma2/platform/sdl3/sdl3_casefix.c"),"-o",str(tmp/name)],check=True)
        subprocess.run([str(tmp/name)],cwd=tmp,check=True)
