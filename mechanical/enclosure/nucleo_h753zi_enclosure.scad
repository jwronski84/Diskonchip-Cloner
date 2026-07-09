/* MD2202-D16 Cloner Enclosure for NUCLEO-H753ZI
   Fits 240x240x220 printers. Default: 220x165 footprint.
   Export: openscad -o part.stl -D 'part="bottom"' nucleo_h753zi_enclosure.scad
*/
$fn=48;
part="assembly"; // bottom, top, buttons, knob, assembly
W=220; D=165; BH=38; TF=18; TR=45; wall=3; r=8;

module rr2(x,y,rad){hull(){translate([rad,rad])circle(rad);translate([x-rad,rad])circle(rad);translate([x-rad,y-rad])circle(rad);translate([rad,y-rad])circle(rad);}}
module rb(x,y,z,rad){linear_extrude(z) rr2(x,y,rad);} 
module cylhole(x,y,d,h=80){translate([x,y,-1])cylinder(d=d,h=h);} 
module recthole(x,y,sx,sy,h=80){translate([x-sx/2,y-sy/2,-1])cube([sx,sy,h]);}
module boss(x,y,h=14,od=10,id=3.2){translate([x,y,wall])difference(){cylinder(d=od,h=h);translate([0,0,-.1])cylinder(d=id,h=h+.2);}}
module insert_boss(x,y,h=28){boss(x,y,h,12,4.2);} 
module vent_side(x){for(i=[0:8])translate([x,35+i*10,18])cube([5,5,13]);}

// Approx board pockets/standoffs. Adjust after measuring exact adapter PCB.
nucleo=[90,110]; adapter=[145,72]; display=[76,50]; zif=[70,24];
nucleo_xy=[20,28]; adapter_xy=[38,83];

module bottom(){
 difference(){
  rb(W,D,BH,r);
  translate([wall,wall,wall])rb(W-2*wall,D-2*wall,BH,r-wall);
  // rear access cutouts: ST-LINK USB, 5V power, microSD, UART/debug slot
  translate([36,D-wall-.2,17])cube([24,wall+1,11],center=true);
  translate([78,D-wall-.2,17])cube([14,wall+1,9],center=true);
  translate([124,D-wall-.2,18])cube([18,wall+1,4],center=true);
  translate([166,D-wall-.2,18])cube([28,wall+1,8],center=true);
  vent_side(-.2); vent_side(W-wall+.2);
 }
 // top-panel heat-set bosses
 for(p=[[14,14],[W-14,14],[14,D-14],[W-14,D-14]]) insert_boss(p[0],p[1]);
 // NUCLEO-H753ZI support standoffs - clearance pocket assumes board mounted lengthwise
 for(p=[[nucleo_xy[0],nucleo_xy[1]],[nucleo_xy[0]+nucleo[0],nucleo_xy[1]],[nucleo_xy[0],nucleo_xy[1]+nucleo[1]],[nucleo_xy[0]+nucleo[0],nucleo_xy[1]+nucleo[1]]]) boss(p[0],p[1],12,9,3.1);
 // adapter/buffer board standoffs below ZIF sockets
 for(p=[[adapter_xy[0],adapter_xy[1]],[adapter_xy[0]+adapter[0],adapter_xy[1]],[adapter_xy[0],adapter_xy[1]+adapter[1]],[adapter_xy[0]+adapter[0],adapter_xy[1]+adapter[1]]]) boss(p[0],p[1],16,9,3.1);
 // display support posts
 for(p=[[60,28],[160,28]]) boss(p[0],p[1],10,8,2.7);
 // cable tie anchors
 for(p=[[22,78],[198,78]]) translate([p[0]-10,p[1]-4,wall])difference(){rb(20,8,5,2);translate([4,2,-.1])cube([12,4,6]);}
}

function th(y)=TF+(TR-TF)*y/D;
module wedge(){polyhedron(points=[[0,0,0],[W,0,0],[W,D,0],[0,D,0],[0,0,TF],[W,0,TF],[W,D,TR],[0,D,TR]],faces=[[0,1,2,3],[4,7,6,5],[0,4,5,1],[1,5,6,2],[2,6,7,3],[3,7,4,0]]);} 
module top(){
 difference(){
  intersection(){wedge(); rb(W,D,TR+2,r);}
  translate([wall,wall,-1])rb(W-2*wall,D-2*wall,TR+2,r-wall);
  // accessible top cutouts
  recthole(70,116,zif[0],zif[1]);   // SOURCE ZIF
  recthole(150,116,zif[0],zif[1]);  // TARGET ZIF
  recthole(110,46,display[0],display[1]);
  cylhole(110,78,7.5);              // encoder
  for(x=[54,84,136,166]) cylhole(x,24,16.5); // buttons
  for(x=[73,91,129,147]) cylhole(x,66,3.2);  // LEDs
  for(p=[[14,14],[W-14,14],[14,D-14],[W-14,D-14]]) cylhole(p[0],p[1],3.4);
 }
 // raised bezels/recess lips
 translate([31,102,th(116)-2])difference(){rb(78,34,3,3);translate([4,5,-1])cube([70,24,5]);}
 translate([111,102,th(116)-2])difference(){rb(78,34,3,3);translate([4,5,-1])cube([70,24,5]);}
 translate([68,18,th(46)-2])difference(){rb(84,58,3,4);translate([4,4,-1])cube([76,50,5]);}
 label(70,139,"SOURCE"); label(150,139,"TARGET"); label(110,91,"MENU");
 label(54,38,"READ"); label(84,38,"CLONE"); label(136,38,"VERIFY"); label(166,38,"WRITE");
}
module label(x,y,t){translate([x,y,th(y)+.5])linear_extrude(.8)text(t,size=5,halign="center",valign="center",font="Liberation Sans:style=Bold");}
module knob(){difference(){union(){cylinder(d=28,h=15);translate([0,0,15])cylinder(d1=28,d2=24,h=3);}translate([0,0,-.1])cylinder(d=6.1,h=13);for(a=[0:20:340])rotate([0,0,a])translate([13,0,9])cube([2,1,12],center=true);}}
module button(label="CLONE"){difference(){rb(24,18,6,3);translate([6,3,-.1])cube([12,12,3]);}translate([12,9,6])linear_extrude(.7)text(label,size=4,halign="center",valign="center",font="Liberation Sans:style=Bold");}
module buttons(){button("READ");translate([30,0,0])button("CLONE");translate([65,0,0])button("VERIFY");translate([105,0,0])button("WRITE");}
module assembly(){bottom();translate([0,0,BH+1])top();translate([110,78,BH+th(78)+5])knob();translate([50,8,BH+4])buttons();}

if(part=="bottom")bottom(); else if(part=="top")top(); else if(part=="knob")knob(); else if(part=="buttons")buttons(); else assembly();
