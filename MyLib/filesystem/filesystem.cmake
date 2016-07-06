SET(RZ_FILESYSTEMAPI
	${RZ_MYLIB}/_base.h
	${RZ_MYLIB}/filesystem/FileApi.h
	${RZ_MYLIB}/filesystem/FileApi.c
)

source_group("filesystem" FILES ${RZ_FILESYSTEMAPI})
message(status "** RZ_FILESYSTEMAPI : ${RZ_FILESYSTEMAPI}")
