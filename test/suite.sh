#!/bin/sh

cleanup () {
  find . -name "*.gcda" -exec rm {} \;
  find . -name "*.gcov" -exec rm {} \;
}

BLITZCC=_release/bin/blitzcc

cleanup

# run the proper suite
$BLITZCC -r test test/all.bb
RESULT=$?
if [ $RESULT -ne 0 ]; then
  echo "Test suite failed. Fix it and then run the coverage generation again."
  exit 1
fi

# test a few of the compiler flags
$BLITZCC -v
$BLITZCC +k
$BLITZCC -k
$BLITZCC -l
$BLITZCC -h

$BLITZCC -r test -a test/all.bb
$BLITZCC -r test -c test/all.bb
$BLITZCC -r test -j test/all.bb

# test for syntax errors
$BLITZCC -r test -j test/syntax-errors/arithmetic-on-type.bb
$BLITZCC -r test -j test/syntax-errors/division-by-zero.bb

# invalid runtime
$BLITZCC -r d3d1 test/all.bb

# generate the report...
ENV=$($BLITZCC -e)
if [ "$ENV" = "test" ]; then
  echo "Generating coverage report..."
  mkdir -p coverage
  gcovr -e '.*stdin.*' --html --html-details -o ./coverage/coverage.html build/
  cleanup
fi

exit 0
