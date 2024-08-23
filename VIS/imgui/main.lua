imgui.SetCursorPosX(10)
imgui.SetCursorPosY(10)
if imgui.Button(" 3D ") then
	seti("draw hlr", 8)
	estack_vis(1)
	set_stage('3d')
	refresh('sm')
end

imgui.SetCursorPosX(300)
imgui.SetCursorPosY(10)
imgui.TextUnformatted(
[[
------------------------------
LUA API: 
------------------------------
rnd,rnd
bnd,bnd
atan2,atan2
pow,pow
cnt,lua_cnt

color,colori
rgb,rgb
hsl,hsl
psz,pixsize
mod,rendermod

dolua,dolua
realphg,realphg
dophg,luadophg

------------------------------
 message
------------------------------
prt,print
msg,msgbox
msgbox,msgbox
getchar,getchar

------------------------------
 vector params
------------------------------
setscaler,setscaler
param,setparam
setf,setfloatmap
getf,getfloatmap
seti,setintmap
geti,getintmap
sets,setstringmap
setv3,setvec3map
vec3,setvec3map2
getv3,getvec3map
setp,setpointmap
getp,getpointmap
movepointn,movepointn
lambda,cclambda
cd3,setcoord3map
copycd3,copycoord3map
look,coordlook

// enity
ent,createent

// model
readobj,readobj
saveobj,saveobj

cell,celli
celli,celli
cells,cells
cellscript,cellscript
curcell,curcell

------------------------------
 stack pm
------------------------------
newe,newedge
push,pushe
pop,pope

pushc,pushcoord
popc,popcoord
coord,calccoord
uz,coorduz
ux,coordux
mor,mirror

pushd,setdir
popd,popdir

comv,commonvert
scomv,searchcomvert
inv,invnorm


round,roundedge
arc,arcedge
edge,edge

wgt,weight
wbnd,weightblend
wdiv,weightdiv
wbndx,weightblendx
wbndz,weightblendz
wbndm,weightblendmirror
wtim,wtimes

ext,extrudedge
ext2,extrudedgex
mov,movedge
mov2,movedgex

scl,scaledge
sclm,scaledgemirror
scl2,scaleeedge2

rot,rotedge
rotm,rotedgemirror
rote,rotonedge
yaw,yawedge
pit,pitchedge
rol,rolledge

radi,radedge

div,div
cls,closedge
sub,subedge
linke,linkedge
union,unionedge
double,doublevnum
pos,setpos

face,face
faceo,faceo

clearsm,clearsm
clearsk,clearstack

terrain,starterrain
place,transform_place
fly,transform_fly
rotcd,rotcoord3
path,astar_path

main,maintest
geom,geom
test,test
scene,scene

------------------------------
psz,pixsize
pix,pixel
pixi,pixeli
pst,pset
linei,linei

tri,triang
ver,addver
trii,triangi

pln,plane
crd,coorddummy
pyramid,pyramid
sphere,sphere
ball,sphere
cylinder,cylinder
cube,cube
box,cube
pt3d,lua_pt3d
ptr,lua_ptr
draw_ptr,lua_ptr
link,link
poly,poly
line,line
curve,drawcurve
drawpset,drawpset
]]
)

imgui.SetCursorPosX(800)
imgui.SetCursorPosY(10)
imgui.TextUnformatted(
[[
------------------------------
PHG API: 
------------------------------
echo
mod
msg
msgbox
rnd
cos
sin
tan
cot
sec
csc
pow
log
exp
sqrt
floor
ceil
abs
min
max
tos
setup
api
lua
python
map
sets
seti
setf
setv3
order
im
bye
on
array
sequ
prop
wak
expr
dump
abe
addd
add
subb
sub
calc
getival
getfval
getstr
getvec3
getrect
cls
dt_sum
dt_avg
dt_gm
dt_mM
dt_VR
dt_SVR
------------------------------
rgb
seti
sets
setv3
clreq
picksm
resetsm
clrsm
draw_md
build_md
draw_prims
draw_norm
draw
gen_feature
showground
bool
coord
clearp
savep
drawpset
pt3d
ptr
link
cylinder
sphere
ball
cube
box
cone
powcone
line
poly
polyface
readobj
saveobj
saveobjs
readstl
savestl
readglb
saveglb
readsmb
readsmba
savesmb
savesmbs
savesm
savesms
sphere1
pipe1
plane1
box1
cylinder1
cone1
------------------------------
gv
la
ls
update
place
move
fly
drawlink
dumpcst
vec
loc
qua
q2pyr
qslerp
crd
lok
lerp
slerp
vis
prj_dir
clr_feature
feature
draw_feature
save_feature
read_feature
save_poly_csv
wt_test
detection_mesh_elbow
detection_mesh_prim
detection_mesh_vol
lerp_mesh
scene_mesh
api
dumps
------------------------------
int Key: progress Value: 1
int Key: draw hlr Value: 8
int Key: detect mesh Value: 8
vec3 Key: UZ Value: (0, 0, 1)
vec3 Key: O Value: (0, 0, 0)
vec3 Key: Z1 Value: (0, 0, 1)
vec3 Key: I Value: (1, 1, 1)
vec3 Key: X1 Value: (1, 0, 0)
vec3 Key: Y1 Value: (0, 1, 0)
vec3 Key: UX Value: (1, 0, 0)
vec3 Key: UY Value: (0, 1, 0)
coord3 Key: RX
coord3 Key: CO
coord3 Key: CI
coord3 Key: CX1
coord3 Key: CY1
coord3 Key: CZ1
coord3 Key: RX90
coord3 Key: AX0
coord3 Key: RY
coord3 Key: RX45
coord3 Key: RY90
coord3 Key: RY45
coord3 Key: RZ
coord3 Key: RZ90
coord3 Key: RZ45
coord3 Key: AY0
coord3 Key: AZ0
]]
)