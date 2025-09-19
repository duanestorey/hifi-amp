
// The outside width of the enclosure
base_width = 150;

// The outside depth of the enclosure
base_depth = 150;

// The outside height of the enclosure
base_height = 80;

base_lid_thickness = 3;
split_amount = 0;

front_back_thickness = 6;
front_overhang = 3;
back_overhang = 0;
back_thickness = 3;
sides_thickness = 8.8;
lcd_cutout_thickness = 2;

screw_recess = 1;
screw_size = 3.5;
screw_hole = 4.4;
screw_offset_z = 12.5;
screw_head = 5;

$fn = 32;

lcd_x = 97.5;
lcd_y = 40.5;
lcd_hole_offset_y = 7.5;
lcd_hole_offset_x = 2.5;
lcd_hole_d = 2.5;

foot_d = 30;
foot_h = 10;
foot_offset = 10;
foot_recess = 1;
foot_recess_size = 0.1;

back_working_area = 20;
front_working_area = 20;
plate_interface_size = 3;
plate_interface_slop = 0.2;
plate_z = 5;
plate_thickness = 2;
plate_grid_spacing = 10;
plate_hole_size = 3.5;

include_front = true;
include_back = true;
include_plate = true;

module draw_lid( pos ) {
    difference() {
        color( "green" ) cube( [ base_width, base_depth - back_thickness, base_lid_thickness ] );
        
        translate( [ sides_thickness/2, screw_offset_z, 0 ] ) {
            cylinder( r=screw_size/2, h=base_lid_thickness);
            
            translate( [0, 0, pos] ) cylinder( r=screw_head/2, h=screw_recess );
        }
        
        translate( [ sides_thickness/2,  base_depth - back_thickness - screw_offset_z, 0 ] ) {
            cylinder( r=screw_size/2, h=base_lid_thickness);
            
            translate( [0, 0, pos] ) cylinder( r=screw_head/2, h=screw_recess );
        }
        
        translate( [ base_width - sides_thickness/2, screw_offset_z, 0 ] ) {
            cylinder( r=screw_size/2, h=base_lid_thickness);
            
            translate( [0, 0, pos] ) cylinder( r=screw_head/2, h=screw_recess );
        }
        
        translate( [ base_width - sides_thickness/2,  base_depth - back_thickness - screw_offset_z, 0 ] ) {
            cylinder( r=screw_size/2, h=base_lid_thickness);
            
            translate( [0, 0, pos] ) cylinder( r=screw_head/2, h=screw_recess );
        }
    }
}

module draw_feet_recess() {
    translate( [ foot_offset + foot_d/2, foot_offset + foot_d/2, 0 ] ) {
        cylinder( h=foot_recess, r=(foot_d/2)+foot_recess_size );
    }
    
     translate( [ base_width - foot_offset - foot_d/2, foot_offset + foot_d/2, 0 ] ) {
        cylinder( h=foot_recess, r=(foot_d/2)+foot_recess_size );
    }
    
     translate( [ base_width - foot_offset - foot_d/2, base_depth - foot_offset - foot_d/2, 0 ] ) {
        cylinder( h=foot_recess, r=(foot_d/2)+foot_recess_size );
    }
    
      translate( [ foot_offset + foot_d/2, base_depth - foot_offset - foot_d/2, 0 ] ) {
        cylinder( h=foot_recess, r=(foot_d/2)+foot_recess_size );
    }
}

module draw_bottom() {
    translate( [ 0, 0, -split_amount ] ) {
        difference() {
            draw_lid( 0 );
            
            draw_feet_recess();
        }
    }
}

module draw_top() {
    translate( [0, 0, base_height - base_lid_thickness + split_amount] ) {
        draw_lid( base_lid_thickness - screw_recess );
    }
}

module draw_front_back( thickness, overhang, pos ) {
    difference() {
            cube( [ base_width + 2*overhang, thickness, base_height + 2*overhang ] );
            
            translate( [ overhang + sides_thickness/2, 0, sides_thickness/2 + overhang + base_lid_thickness] ) {
                rotate( [270, 0, 0 ] ) cylinder( r=screw_size/2, h=thickness );
                translate( [ 0, pos, 0 ] ) {
                    rotate( [270, 0, 0] ) cylinder( r=5/2, h=1);
                }
            }
            
            translate( [ base_width + overhang - sides_thickness/2, 0, sides_thickness/2 + overhang + base_lid_thickness] ) {
                rotate( [270, 0, 0 ] ) cylinder( r=screw_size/2, h=thickness );
                
                translate( [ 0, pos, 0 ] ) {
                    rotate( [270, 0, 0] ) cylinder( r=5/2, h=1);
                }
            }
            
                        
            translate( [ overhang + sides_thickness/2, 0, base_height - sides_thickness/2 + overhang - base_lid_thickness] ) {
                rotate( [270, 0, 0 ] ) cylinder( r=screw_size/2, h=thickness );
                
                translate( [ 0, pos, 0 ] ) {
                    rotate( [270, 0, 0] ) cylinder( r=5/2, h=1);
                }
            }
            
             translate( [ base_width + overhang - sides_thickness/2, 0, base_height - sides_thickness/2 + overhang - base_lid_thickness] ) {
                rotate( [270, 0, 0 ] ) cylinder( r=screw_size/2, h=thickness );
                
                translate( [ 0, pos, 0 ] ) {
                    rotate( [270, 0, 0] ) cylinder( r=5/2, h=1);
                }
            }
    }
}

module draw_lcd_holes() {
    lcd_start_x = (base_width + 2*front_overhang - lcd_x)/2;
    lcd_start_y = (base_height + 2*front_overhang - lcd_y )/2;
    
    translate( [ lcd_start_x + lcd_hole_offset_x, 0, lcd_start_y - lcd_hole_offset_y ] ) {
        rotate( [270, 0, 0 ] ) cylinder( h=front_back_thickness, r=lcd_hole_d/2 );
    }
    
    translate( [ lcd_start_x + lcd_x - lcd_hole_offset_x, 0, lcd_start_y - lcd_hole_offset_y ] ) {
        rotate( [270, 0, 0 ] ) cylinder( h=front_back_thickness, r=lcd_hole_d/2 );
    }
    
    translate( [ lcd_start_x + lcd_hole_offset_x, 0, lcd_start_y + lcd_y + lcd_hole_offset_y ] ) {
        rotate( [270, 0, 0 ] ) cylinder( h=front_back_thickness, r=lcd_hole_d/2 );
    }
    
    translate( [ lcd_start_x + lcd_x - lcd_hole_offset_x, 0, lcd_y + lcd_start_y + lcd_hole_offset_y ] ) {
        rotate( [270, 0, 0 ] ) cylinder( h=front_back_thickness, r=lcd_hole_d/2 );
    }
}

module draw_lcd() {
    translate( [ (base_width + 2*front_overhang - lcd_x)/2,     lcd_cutout_thickness, (base_height + 2*front_overhang - lcd_y )/2 ] ) {
        cube( [ lcd_x, front_back_thickness - lcd_cutout_thickness, lcd_y ] );
        
       // translate( [ 6, 5, lcd_y ] ) cube( [ 50, 10, 5 ] );
    }
    
    cutout_x = 71;
    cutout_y = 21;
    
    translate( [ (base_width + 2*front_overhang - cutout_x)/2, 0, (base_height + 2*front_overhang - cutout_y )/2 ] ) {
        cube( [ cutout_x, lcd_cutout_thickness, cutout_y ] );
    }
}

module draw_front_with_standoffs() {
    draw_front_back( front_back_thickness, front_overhang, 0 );
}

module draw_front() {
    translate( [-front_overhang, -front_back_thickness - split_amount, -front_overhang] ) {
        difference() {

            color( "blue" ) draw_front_with_standoffs();
            
            draw_lcd();
            
            draw_lcd_holes();
        }
      
    }
}

module draw_back() {
    translate( [-back_overhang, base_depth + split_amount - back_thickness, -back_overhang ] ) {
        //color( "blue" ) cube( [ base_width + 2*back_overhang, back_thickness, base_height + 2*back_overhang ] );
        
        draw_front_back( back_thickness, back_overhang, back_thickness - screw_recess );
    }
}

module draw_one_side() {
        difference() {
            color( "red" ) cube( [ sides_thickness, base_depth - back_thickness, base_height - 2*base_lid_thickness ] ); 
            
            translate( [ sides_thickness/2, 0, sides_thickness/2 ] ) {
                rotate( [ 270, 0, 0 ] ) cylinder( h=5, r=screw_hole/2 );
            }
            
            translate( [ sides_thickness/2, 0, base_height - 2*base_lid_thickness - sides_thickness/2 ] ) {
                rotate( [ 270, 0, 0 ] ) cylinder( h=5, r=screw_hole/2 );
            }
            
            translate( [ sides_thickness/2, base_depth - back_thickness, sides_thickness/2 ] ) {
                rotate( [ 90, 0, 0 ] ) cylinder( h=5, r=screw_hole/2 );
            }
            
            translate( [ sides_thickness/2, base_depth - back_thickness, base_height - 2*base_lid_thickness - sides_thickness/2 ] ) {
                rotate( [ 90, 0, 0 ] ) cylinder( h=5, r=screw_hole/2 );
            }
            
            translate( [ sides_thickness/2, screw_offset_z, 0 ] ) {
                rotate( [ 0, 0, 0 ] ) cylinder( h=5, r=screw_hole/2 );
            }
            
            translate( [ sides_thickness/2, screw_offset_z, base_height - 2*base_lid_thickness ] ) {
                rotate( [ 0, 180, 0 ] ) cylinder( h=5, r=screw_hole/2 );
            }
            
            translate( [ sides_thickness/2, base_depth - back_thickness - screw_offset_z, 0 ] ) {
                rotate( [ 0, 0, 0 ] ) cylinder( h=5, r=screw_hole/2 );
            }
            
            translate( [ sides_thickness/2, base_depth - back_thickness - screw_offset_z, base_height - 2*base_lid_thickness ] ) {
                rotate( [ 0, 180, 0 ] ) cylinder( h=5, r=screw_hole/2 );
            }
        }
}

module draw_plate_interface( offset ) {
    plate_depth = base_depth - front_working_area - back_working_area + 2*plate_interface_slop;
    translate( [ 0, front_working_area - plate_interface_slop, plate_z -  plate_interface_slop ] ) {
        translate( [ offset, 0, 0 ] ) cube( [ plate_interface_size + plate_interface_slop, plate_depth, plate_thickness + 2*plate_interface_slop ] );
    }
}

module draw_left() {
    translate( [ -split_amount, 0, base_lid_thickness ] ) {
        difference() {
            draw_one_side();
            
            draw_plate_interface( sides_thickness - plate_interface_size - plate_interface_slop );
        }
    }
}

module draw_right() {
    translate( [ base_width + split_amount - sides_thickness, 0, base_lid_thickness ] ) {
        difference() {
            draw_one_side();
            
            draw_plate_interface( 0 );
        }
    }
}

module draw_feet() {
    translate( [ 0, 0, -foot_h - 2*split_amount ] ) {
        translate( [ foot_d/2 + foot_offset, foot_d/2 + foot_offset, 0 ] ) {
            cylinder( h=foot_h, r=foot_d/2 );
        }
        
        translate( [ base_width - foot_d/2 - foot_offset, foot_d/2 + foot_offset, 0 ] ) {
            cylinder( h=foot_h, r=foot_d/2 );
        }
        
        translate( [ base_width - foot_d/2 - foot_offset, base_depth - foot_d/2 - foot_offset, 0 ] ) {
            cylinder( h=foot_h, r=foot_d/2 );
        }
        
        translate( [ foot_d/2 + foot_offset, base_depth - foot_d/2 - foot_offset, 0 ] ) {
            cylinder( h=foot_h, r=foot_d/2 );
        }
    }
}

module draw_plate() {
    translate( [0, 0, base_lid_thickness ] ) {
        plate_depth = base_depth  - back_working_area - front_working_area;
        plate_width = base_width - 2*sides_thickness + plate_interface_size*2;
        total_x = floor( plate_width/plate_grid_spacing );
        total_y = floor( plate_depth/plate_grid_spacing );
        left_over_x = plate_width - total_x*plate_grid_spacing;
        left_over_y = plate_depth - total_y*plate_grid_spacing;
    
        translate( [ sides_thickness - plate_interface_size/2, front_working_area, plate_z ] ) {
            difference() {
                cube( [ plate_width, plate_depth, plate_thickness ] );
            
                for( x = [0:1:total_x-1] ) {
                    for ( y = [0:1:total_y-1] ) {
                        translate( [ left_over_x/2 + 5 + x*plate_grid_spacing, left_over_y/2 + 5 + y*plate_grid_spacing, 0 ] ) {
                            cylinder( h=2, r=plate_hole_size/2 );
                        }
                    }
                }
            }
        }
    }
}

draw_bottom();
draw_top();
if ( include_front ) {
    draw_front();

}
if ( include_back ) draw_back();
draw_left();
draw_right();
draw_feet();
if ( include_plate ) draw_plate();
