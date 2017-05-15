# ATan2# ( y#, x# )

## Parameters

- y = y coordinate
- x = x coordinate

## Description

ATan2 gives the angle between the positive x-axis and a vector from the point (0,0) to the point (x,y).
One common use is in 2d graphics. Suppose you have two objects and you want to aim the first at the second.
ATan2( y2 - y1, x2 - x1 ) gives the proper orientation for object1.
You can use this angle to select an appropriately rotated image.
Notice the reverse order, ATan2( y, x ) rather than ATan2( x, y).
ATan2( y, x ) is analogous to ATan( y / x), but covers 360 degrees.
The angle satisfies:  -180 &lt; ATan2 &lt;= +180
