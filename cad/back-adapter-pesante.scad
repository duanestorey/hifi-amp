$fn = 64;

width = 433/2;
height = 80;
depth = 6;
border = 8;

split = 30;

m3_clearance = 3.4;
m3_countersunk = 5.6;
m3_countersink_depth = 2;

hole_location = border + m3_clearance + 2;

show_holder = true;
show_backing = false;

module draw_countersunk_hole() {
    cylinder( h=border, r = m3_clearance/2 );
    cylinder( h=m3_countersink_depth, r=m3_countersunk/2 );
}

module show_holder() {
    translate( [ border, 0, -5 ] ) cube( [ width - 2 * border, 2, 5 ] );
    
    difference() {
        cube( [ width, height, depth ] );
    
        translate( [ border, border, 0 ] ) {
            cube( [ width - border*2, height - border * 2, depth ] );
        }
    
        translate( [ border, hole_location, depth/2 ] ) {
            rotate( [ 0, 270, 0 ] ) {
                draw_countersunk_hole();
           // cylinder( h=border, r = m3_clearance/2 );
            }
        }
    
        translate( [ border, height - hole_location, depth/2 ] ) {
            rotate( [ 0, 270, 0 ] ) {
                draw_countersunk_hole();
            }
        }
    
        translate( [ width - border, hole_location, depth/2 ] ) {
            rotate( [ 0, 90, 0 ] ) {
                draw_countersunk_hole();
            }
        }
    
        translate( [ width - border, height - hole_location, depth/2 ] ) {
            rotate( [ 0, 90, 0 ] ) {
                draw_countersunk_hole();
            }
        }
    
        translate( [ border/2, border/2, 0 ] ) {
            rotate( [ 0, 0, 0 ] ) {
                draw_countersunk_hole();
            }
        }
    
        translate( [ width - border/2, border/2, 0 ] ) {
            rotate( [ 0, 0, 0 ] ) {
                draw_countersunk_hole();
            }
        }
    
        translate( [ border/2, height - border/2, 0 ] ) {
            rotate( [ 0, 0, 0 ] ) {
                draw_countersunk_hole();
            }
        }
    
        translate( [ width - border/2, height - border/2, 0 ] ) {
            rotate( [ 0, 0, 0 ] ) {
                draw_countersunk_hole();
            }
        }
        
        translate( [ border/2, height/4, 6 ] ) {
            rotate( [ 0, 180, 0 ] ) {
                draw_countersunk_hole();
            }
        }
        
        translate( [ border/2, 3*height/4, 6 ] ) {
            rotate( [ 0, 180, 0 ] ) {
                draw_countersunk_hole();
            }
        }
        
        translate( [ width - border/2, height/4, 6 ] ) {
            rotate( [ 0, 180, 0 ] ) {
                draw_countersunk_hole();
            }
        }
        
        translate( [ width - border/2, 3*height/4, 6 ] ) {
            rotate( [ 0, 180, 0 ] ) {
                draw_countersunk_hole();
            }
        }
    }
}

module show_back() { 
    translate( [ 0, 0, split ] ) {
        difference() {
            color( "red" )cube( [ width, height, 6 ] );
            
            translate( [ border/2, border/2, 6 ] ) {
                rotate( [ 0, 180, 0 ] ) {
                    draw_countersunk_hole();
                }
            }
            
            translate( [ width - border/2, border/2, 6 ] ) {
                rotate( [ 0, 180, 0 ] ) {
                    draw_countersunk_hole();
                }
            }
            
            translate( [ border/2, height - border/2, 6 ] ) {
                rotate( [ 0, 180, 0 ] ) {
                    draw_countersunk_hole();
                }
            }
            
            translate( [ width - border/2, height - border/2, 6 ] ) {
                rotate( [ 0, 180, 0 ] ) {
                    draw_countersunk_hole();
                }
            }
        }
    }
}

if ( show_holder ) {
    show_holder();
}

if ( show_backing ) {
    show_back();
}