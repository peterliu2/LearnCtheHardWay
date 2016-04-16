#! /bin/bash
# Program:
#	Test devpkg 
#
set -e

TEST_DIR=${HOME}/workspace/LCTHW/ex26/devpkg/test
TEST_FILE=${TEST_DIR}/test.sh
CMD_DIR=${TEST_DIR}/..
CMD=${CMD_DIR}/devpkg
DATABASE_DIR=/usr/local/.devpkg
DEPENDS_FILE=${TEST_DIR}/peter.DEPENDS

## test pattern
TEST_F_1="-F http://apache.stu.edu.tw/apr/apr-1.5.2.tar.gz"
TEST_F_2="-F http://apache.stu.edu.tw/apr/apr-1.5.2.tar.bz2"
TEST_F_3="-F http://apache.stu.edu.tw/apr/apr-util-1.5.4.tar.gz"
TEST_F_4="-F http://apache.stu.edu.tw/apr/apr-util-1.5.4.tar.bz2"
TESF_F_5="-F git://git.kernel.org/pub/scm/devel/sparse/sparse.git"

TEST_B_1="-B http://apache.stu.edu.tw/apr/apr-1.5.2.tar.gz"
TEST_B_2="-B http://apache.stu.edu.tw/apr/apr-1.5.2.tar.bz2"
TEST_B_3="-c --with-apr=/usr/local/apr -B http://apache.stu.edu.tw/apr/apr-util-1.5.4.tar.gz"
TEST_B_4="-c --with-apr=/usr/local/apr -B http://apache.stu.edu.tw/apr/apr-util-1.5.4.tar.bz2"
TEST_B_5="-B git://git.kernel.org/pub/scm/devel/sparse/sparse.git"

TEST_I_1='-I http://apache.stu.edu.tw/apr/apr-1.5.2.tar.gz'
TEST_I_2='-I http://apache.stu.edu.tw/apr/apr-1.5.2.tar.bz2'
TEST_I_3='-c --with-apr=/usr/local/apr -I http://apache.stu.edu.tw/apr/apr-util-1.5.4.tar.gz'
TEST_I_4='-c --with-apr=/usr/local/apr -I http://apache.stu.edu.tw/apr/apr-util-1.5.4.tar.bz2'
TEST_I_5='-I git://git.kernel.org/pub/scm/devel/sparse/sparse.git'

# remove database
remove_database () {
	rm -fr ${DATABASE_DIR}
}

# setup database
setup_database () {
	${CMD} -S
	[ $? -eq 0 ] && echo "Setup database OK" || { echo "Setup database Fail" >&2; exit 1; }
}

# list database content
list_database_content () {
	${CMD} -L
	[ $? -eq 0 ] && echo "list OK" || { echo "list Failed" >&2; exit 1; }
}

# test -I
# argument: complete parameter list
test_I () {
	echo "Stat -I testing..."
	for test in "$@"
	do 
		${CMD} ${test} 1>/dev/null 2>&1 
		[ $? -eq 0 ] && echo "test -I OK: ${test}" || { echo "test -I NG: ${test}" >&2; exit 1; }
	done
}

# test -d
# argument: *.DEPRNDS file path
test_d () {
	echo "Starting -d testing..."
	${CMD} -d ${1} 1>/dev/null 2>&1
	[ $? -eq 0 ] && echo "test -d OK: ${test}" || { echo "test -d NG: ${test}" >&2; exit 1; }
}

#tesd -F -B
# argument: single pair fetch and buile parameter
test_F_B () {
	echo "Stat -F -B testing..."

	eval s1="$1"
	eval s2="$2"
	# -F
	${CMD} $s1 1>/dev/null 2>&1 
	[ $? -eq 0 ] && echo "test -F OK: $s1" || { echo "test -F NG: $s1" >&2; exit 1; }
	
	# -B
	${CMD} $s2 1>/dev/null 2>&1 
	[ $? -eq 0 ] && echo "test -B OK: $s2" || { echo "test -B NG: $s2" >&2; exit 1; }
}

###
### -I test
###

## check CMD is exit and executable
[ -x ${CMD} ] && echo "\"${CMD}\" is OK" || { echo "\"${CMD}\" doesn't exit or executable." >&2; exit 1; }

## rm database
remove_database

## setup database
setup_database

## list database contents
list_database_content

## start -I test
test_I "${TEST_I_1}" "${TEST_I_2}" "${TEST_I_3}" "${TEST_I_4}" "${TEST_I_5}"

## list database contents
echo "database contents - a package per line"
list_database_content

###
### Start -d test
###

## check DEPENDS_FILE is exit and readable
[ -r ${DEPENDS_FILE} ] && echo "\"${DEPENDS_FILE}\" is OK" || { echo "\"${DEPENDS_FILE}\" doesn't exit or readable." >&2; exit 1; }

## remove database
remove_database

## setup database
setup_database

## list database contents
list_database_content

## start -d test 
test_d ${DEPENDS_FILE}

## list database contents
echo "database contents - a package per line"
list_database_content


###
### Start -F -B test
###

## remove database
remove_database

## setup database
setup_database

## list database contents
list_database_content

## start -F- B test 
test_F_B "\${TEST_F_1}" "\${TEST_B_1}"
test_F_B "\${TEST_F_2}" "\${TEST_B_2}"
test_F_B "\${TEST_F_3}" "\${TEST_B_3}"
test_F_B "\${TEST_F_4}" "\${TEST_B_4}"
#test_F_B "\${TEST_F_5}" "\${TEST_B_5}"

## list database contents
echo "database contents - a package per line"
list_database_content

## end test
echo "Test finisk OK"
exit 0
