mesh=LoadAnimMesh( base_mesh$ ) ;anim sequence 0.
ExtractAnimSeq( mesh,0,30 ) ;anim sequence 1: frames 0...30 are 'run'
ExtractAnimSeq( mesh,31,40 ) ;anim sequence 2: frames 31...40 are 'jump' etc  etc...

Animate mesh,3,1,2 ;play one-shot jump anim