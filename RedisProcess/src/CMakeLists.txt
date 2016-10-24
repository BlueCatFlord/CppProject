ADD_EXECUTABLE(RedisProcess
    main.cpp
    except/RedisException.h
    except/RedisException.cpp
    config/INIConfig.h
    config/INIConfig.cpp
    RedisProcess.cpp
    RedisProcess.h
)

TARGET_LINK_LIBRARIES(RedisProcess
    hiredis
    log4cplus
)