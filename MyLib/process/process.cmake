SET(RZ_PROCESSAPI
	${RZ_MYLIB}/_base.h
	${RZ_MYLIB}/process/ProcessApi.h
	${RZ_MYLIB}/process/ProcessApi.c
)

source_group("process" FILES ${RZ_PROCESSAPI})
message(status "** RZ_PROCESSAPI : ${RZ_PROCESSAPI}")
