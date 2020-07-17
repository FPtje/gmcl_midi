#!/bin/sh
premake5 --os=windows --file=BuildProjects.lua vs2010
premake5 --os=windows --file=BuildProjects.lua vs2012
premake5 --os=windows --file=BuildProjects.lua vs2017
premake5 --os=macosx  --file=BuildProjects.lua gmake
premake5 --os=linux   --file=BuildProjects.lua gmake
