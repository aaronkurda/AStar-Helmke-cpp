project(Scheduling)

set(TEST_HEADERS
     Test/SearchAircraftTest.h
	 Test/SearchGuestTest.h
	 Test/TestAStar.h
	 Test/TestFileInput.h
	 Test/TestGuestArray.h
	 Test/TestSequence.h
	 Test/TestAStarAcft.h
	)

	set(TEST_SOURCES
     Test/SearchAircraftTest.cxx
	 Test/SearchGuestTest.cxx
	 Test/TestAStar.cxx
	 Test/TestAStarWettbewerb.cxx
	 Test/TestFileInput.cxx
	 Test/TestGuestArray.cxx
	 Test/TestSequence.cxx
	 Test/TestAStarAcft.cxx
	 Test/TestAStarAcftWettbewerb.cxx
	)


set(Scheduling_SOURCES
      Aircraft.cxx
      AStar.cxx
	  AStarAcft.cxx
	  EvalSeqAircraft.cxx
	  EvalSequence.cxx
      main.cxx
      GuestArray.cxx
      Guest.cxx
      FileInput.cxx
      EvalSequence.cxx
	  QualityFuncAircraft.cxx
	  QualityFuncStatistics.cxx
	  TakeSelect.cxx
	  zeitmess.cxx
		${TEST_SOURCES}
	)

set(Scheduling_HEADERS
    Aircraft.h
    AStar.h
	AStarAcft.h
	BaseDir.h
	dosFarben.h
	EvalSeqAircraft.h
	EvalSequence.h
    GuestArray.h
    Guest.h
    FileInput.h
    EvalSequence.h
	QualityFuncAircraft.h
	QualityFuncStatistics.h
	TakeSelect.h
	TakeSelectSearchParam.h
	testMakros.h
	zeitmess.h
    ${TEST_HEADERS}
	)

set(Scheduling_SOURCES_TEMP
	QualityFuncBase.h
	QualityFuncBase.cxx
	)
	
	
# dann kann man es gleich in Visual Studio bearbeiten
set(Scheduling_INFO
	Data/scheduling.ids
	)
	
source_group("Source Files\\Test" FILES ${TEST_SOURCES})
source_group("Header Files\\Test" FILES ${TEST_HEADERS})
add_executable(Scheduling  ${Scheduling_SOURCES} ${Scheduling_HEADERS} ${Scheduling_SOURCES_TEMP} ${Scheduling_INFO})
set_source_files_properties(${Scheduling_SOURCES_TEMP} PROPERTIES HEADER_FILE_ONLY TRUE)


# Test-Code in /Test und aus den Testdateien wird auch ein Verzeichnis hoeher benoetigt.
target_include_directories(Scheduling PUBLIC ${Scheduling_SOURCE_DIR} ${Scheduling_SOURCE_DIR}/Test ${liblogtrace_SOURCE_DIR})
target_link_libraries(Scheduling  liblogtrace)

