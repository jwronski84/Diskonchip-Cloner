// MD2202-D16 DiskOnChip Cloner enclosure
// Fits 240x240x220 mm build volume. Default footprint: 220x160 mm.
$fn=48;
part="assembly"; // assembly,bottom,top,knob,buttons
X=220; Y=160; H=34; wall=3; r=8;
front=12; rear=34;

module rr2(x,y,rad){hull(){translate([rad,rad])circle(rad);translate([x-rad,rad])circle(rad);translate([x-rad,y-rad])circle(rad);translate([rad,y-rad])circle(rad);}}
module rb(x,y,z,rad){linear_extrude(z) rr2(x,y,rad);}
module boss(x,y,h=24){translate([x,y,wall])difference(){cylinder(h=h,d=12);translate([0,0,-.2])cylinder(h=h+.4,d=4.2);}}
module hole(x,y,d){translate([x,y,-1])cylinder(h=80,d=d);} 
module rect_hole(x,y,w,h){translate([x-w/2,y-h/2,-1])cube([w,h,80]);}
function th(y)=front+(rear-front)*y/Y;

module bottom(){
 difference(){
  rb(X,Y,H,r);
  translate([wall,wall,wall])rb(X-2*wall,Y-2*wall,H,r-wall);
  translate([75,Y-wall-.2,13])cube([14,wall+1,7],center=true); // USB-C
  translate([110,Y-wall-.2,13])cube([12,wall+1,8],center=true); // power
  translate([145,Y-wall-.2,15])cube([16,wall+1,4],center=true); // SD
  for(i=[0:7]){translate([-.2,36+i*10,18])cube([wall+1,5,12]);translate([X-wall+.2,36+i*10,18])cube([wall+1,5,12]);}
 }
 for(p=[[14,14],[206,14],[14,146],[206,146],[35,35],[185,35],[35,125],[185,125]]) boss(p[0],p[1]);
}

module top_blank(){polyhedron(points=[[0,0,0],[X,0,0],[X,Y,0],[0,Y,0],[0,0,front],[X,0,front],[X,Y,rear],[0,Y,rear]],faces=[[0,1,2,3],[4,7,6,5],[0,4,5,1],[1,5,6,2],[2,6,7,3],[3,7,4,0]]);} 
module zif(x,y){rect_hole(x,y,68,22);translate([x-38,y-15,th(y)-3])difference(){rb(76,30,4,3);translate([4,4,-.1])rb(68,22,5,2);}}
module label(x,y,t){translate([x-27,y-5,th(y)+.5])rb(54,10,1,2);translate([x,y,th(y)+1.7])linear_extrude(.8)text(t,size=5,halign="center",valign="center",font="Liberation Sans:style=Bold");}
module top(){
 difference(){
  intersection(){top_blank(); rb(X,Y,rear+2,r);} 
  translate([wall,wall,-.1])rb(X-2*wall,Y-2*wall,rear+1,r-wall);
  rect_hole(X/2,45,70,47); zif(63,106); zif(157,106);
  for(x=[60,92,128,160]) hole(x,32,16.4);
  hole(110,72,7.2);
  for(p=[[14,14],[206,14],[14,146],[206,146]]) hole(p[0],p[1],3.4);
 }
 translate([X/2-39,45-27,th(45)-.5])difference(){rb(78,55,3,4);translate([4,4,-.1])rb(70,47,4,2);} 
 label(63,129,"SOURCE"); label(157,129,"TARGET");
}
module knob(){difference(){union(){cylinder(h=16,d=28);translate([0,0,15])cylinder(h=3,d1=28,d2=25);}translate([0,0,-.2])cylinder(h=14,d=6.1);for(a=[0:20:340])rotate([0,0,a])translate([13,0,8])cube([2,1,12],center=true);}}
module cap(txt){difference(){translate([-12,-9,0])rb(24,18,6,3);translate([0,0,-.1])cube([12,12,3],center=true);}translate([0,0,6])linear_extrude(.7)text(txt,size=4,halign="center",valign="center",font="Liberation Sans:style=Bold");}
module buttons(){cap("READ");translate([32,0,0])cap("CLONE");translate([68,0,0])cap("VERIFY");translate([104,0,0])cap("WRITE");}
module assembly(){bottom();translate([0,0,H+1])top();translate([110,72,H+th(72)+6])knob();translate([54,18,H+10])buttons();}
if(part=="bottom")bottom();else if(part=="top")top();else if(part=="knob")knob();else if(part=="buttons")buttons();else assembly();
