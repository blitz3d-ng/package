# AddAnimSeq ( entity,length )

## Parameters</h1>

| Name   | Description       |
| ------ | ----------------- |
| entity | an entity handle  |
| length | an entity handle  |

## Description

Creates an animation sequence for an entity. This must be done before any  animation keys set by SetAnimKey can be used in  an actual animation however this is optional. You may use it to "bake" the frames you have added previously using SetAnimKey.

Returns the animation sequence number added.
