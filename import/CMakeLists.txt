add_subdirectory(QArchive)

set(subdirectories QSourceHighliter)

foreach(subdir ${subdirectories})
    add_subdirectory(${subdir})
    target_include_directories(${subdir} PUBLIC ${subdir})
endforeach()
