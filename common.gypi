{
    'variables': {
        'library%': 'static_library',

        'conditions': [
            # A flag for POSIX platforms
            ['OS=="win"', {
              'os_posix%': 0,
            }, {
              'os_posix%': 1,
            }]
        ]
    }
}