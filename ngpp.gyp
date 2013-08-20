{
    'targets': [
        {
            'target_name': 'ngpp',
            'type': 'static_library',
            'xcode_settings': {
                'OTHER_CPLUSPLUSFLAGS': [
                    '-stdlib=libc++',
                    '-std=c++11'
                ],
                'OTHER_LDFLAGS': [
                    '-stdlib=libc++',
                    '-std=c++11'
                ]
            }
        },
        {
            'target_name': 'game',
            'type': 'executable',
            'xcode_settings': {
                'OTHER_CPLUSPLUSFLAGS': [
                    '-stdlib=libc++',
                    '-std=c++11'
                ],
                'OTHER_LDFLAGS': [
                    '-stdlib=libc++',
                    '-std=c++11'
                ]
            },
            'dependencies': [
                'deps/SDL2/sdl.gyp:SDL2'
            ],
            'include_dirs': [
                '.',
                'deps/SDL2/include',
                'deps/glm/include'
            ],
            'sources': [
                'game.cc',
                'common.gypi',
            ]
        },
        {
            'target_name': 'unit_tests',
            'type': 'executable',
            'defines': [
                'GTEST_USE_OWN_TR1_TUPLE=1',
                'GTEST_HAS_TR1_TUPLE=1',
                'GTEST_LANG_CXX11=1'
            ],
            'xcode_settings': {
                'OTHER_CPLUSPLUSFLAGS': [
                    '-stdlib=libc++',
                    '-std=c++11'
                ],
                'OTHER_LDFLAGS': [
                    '-stdlib=libc++',
                    '-std=c++11'
                ]
            },
            'dependencies': [
                #'ngpp',
                'deps/gtest/gtest.gyp:gtest',
                'deps/gtest/gmock.gyp:gmock'
            ],
            'include_dirs': [
                '.',
                'deps/glm/include'
            ],
            'sources': [
                'test/tests.cc',
                'common.gypi'
            ]
        }
    ]
}