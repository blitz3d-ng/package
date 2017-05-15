
# Tan# ( degrees# )

## Parameters

- degrees# = angle in degrees.

## Description

Tangent of an angle. The angle is measured in degrees.
For angles between 0 and 90 degrees this is defined by the sides of a right triangle. The tangent is the side opposite the angle divided by the side adjacent to the angle.In general, tangent is defined as sine divided by cosine.
The positive y-axis corresonds to +90 degrees. This is a common source of confusion in Blitz. With screen coordinates ( pixels ) the y-axis points downward. But in the 3d world the y-axis typically points upward.
Another possible snag is the size of the angle. In principle, the tangent function repeats every 180 degrees. So Tan(-180), Tan(0), Tan(180), Tan(360) etc. should all be exactly the same. But in practice the accuracy decreases as the angle gets farther away from zero.
