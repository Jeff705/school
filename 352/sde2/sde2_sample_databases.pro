/* databases */

needs([
[ece2090,1,m,13,16],
[ece2090,2,tu,13,16],
[ece2090,3,w,13,16],
[ece2090,4,f,9,12],

[ece2060,1,tu,11,14],
[ece2060,2,tu,14,17],
[ece2060,3,w,9,12],
[ece2060,4,f,13,16],

[ece3520,1,tu,11,14],
[ece3520,2,tu,14,17],

[ece4420,1,w,13,16],
[ece4420,2,f,13,16]]).

resources([
[joe, [ece2090,ece2010,ece3520,ece4420],[]],
[sam, [ece2010,ece4420],[]],
[pete, [ece3520,ece4420],[]],
[randi, [ece2090],[]],
[gwen, [ece2090,ece2010],[]]]).

resources2([
[joe, [ece2090,ece2010,ece3520,ece4420],[[m,13,16]]],
[sam, [ece2010,ece4420],[]],
[pete, [ece3520,ece4420],[]],
[randi, [ece2090],[]],
[gwen, [ece2090,ece2010],[]]]).

resources3([
[joe, [ece2010,ece3520,ece4420],[[m,13,16]]],
[sam, [ece2010,ece4420],[]],
[pete, [ece3520,ece4420],[]],
[gwen, [ece2095,ece2010],[]]]).

needs4([
[ece2090,1,m,13,16],
[ece2060,1,tu,11,14],
[ece3520,1,tu,11,14],
[ece4420,2,f,13,16]]).

resources4([
[joe, [ece2090],[]],
[sam, [ece2060,ece4420],[]],
[pete, [ece3520,ece4420],[]],
[randi, [ece4420],[]]]).

needs5([
[ece2090,1,m,13,16],
[ece2060,1,tu,11,14]]).

resources5([
[joe, [ece2090],[]],
[sam, [ece2060],[]]]).

needs6([
[ece2090,1,m,13,16],
[ece2080,1,tu,11,14],
[ece3525,1,tu,11,14],
[ece4420,2,f,13,16]]).

resources6([
[joe, [ece2090],[]],
[sam, [ece2060,ece4420],[]],
[pete, [ece3520,ece4420],[]],
[randi, [ece4420],[]]]).
