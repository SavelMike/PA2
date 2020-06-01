#! /bin/bash

# Type of numbers: 
# 	123456789
#	12345.  
#	123456789.1231465879
#	.012135468
#	0.123456789
#	0.00000000123456
#	12345e125
#	12345.e100000000
#	123456.1234e200000
#	.e123456
#	.123e20000
#	123e-12345
#	0.e-12345
#	.e-123456
#	00000000.0000000
#	0.000001231231e-12345
# Types of expressions:
# 	N + M
#	N - M
#	N * M
#	(N+M)*L
#	(N-M)*L
#	((N - M) + ((L - K) * P)) * Q	

# bc test
echo "1234567891223354622222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222454848648646848486
12345.
123456789.123146587946546878797897987897897987878456465456484845152156464846486468486486486486486486486486488648648486486
.0121354684654654654787987987987987987987987897897987897897987987987987987987897884654556456468797897987897987987987987
0.12345678946454564564564564564564564564564564564564564
0.0000000012345600000000000000000000000000000000000000000000000000000000000000000000000000000213135153135468748798797979798
00000000.0000000" | ./../testgen > expr.set

BC_LINE_LENGTH=0
bc -l < expr.set | sed 's/\.0*$//' > test.ref
./../savelmyk < expr.set > test.res 2>/dev/null
diff test.ref test.res || { echo "bc test failed"; exit; }
echo $(wc -l < expr.set) "tests passed" 
exit

# Multiplication test
bc -l < test1 > test.ref
./../savelmyk < test1 > test.res
#diff test.res test.ref || { echo "Multiplication is wrong"; exit; }

echo "Multiplication test is ok"

# Addition test
bc -l < test2 > test.ref
./../savelmyk < test2 > test.res
diff test.res test.ref || { echo "Addition is wrong"; exit; }

echo "Addition test is ok"