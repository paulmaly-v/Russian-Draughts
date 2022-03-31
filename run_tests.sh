# #!/bin/bash

# if [ -d $PWD/.build/coverage_report ]; then
# cd .
# else
#   if [ -f $PWD/.build ]; then
#   cd .
#   else
#     mkdir .build
#     cd .build
#     cmake ..
#     cmake --build .
#     cd ..
#   fi
#   cd .build
#   ./TESTS
#   mkdir coverage_report
#   sudo apt-get install gcovr
#   gcovr -r ../ -e ../googletest-1.10.x --html --html-details -o coverage_report/result.html
#   cd ..
#   mv $PWD/.build/coverage_report/result.src_view.h.html $PWD
# fi
