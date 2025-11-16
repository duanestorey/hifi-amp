speaker_width = 200;
speaker_height = 240;
speaker_depth = 220;

wall_thickness = 12;

module draw_sides( sides ) {
        if ( sides == -1 ) {
             translate( [ -speaker_width/2, 0, 0 ] ) {
                cube( [ wall_thickness, speaker_height, speaker_depth ], true );
             }
        } else if ( sides == 1 ) {
            translate( [ speaker_width/2, 0, 0 ] ) {
                cube( [ wall_thickness, speaker_height, speaker_depth ], true );
             }
        }   
}

module draw_top( which ) {
        if ( which == -1 ) {
             translate( [ 0, speaker_height*0.75, 0 ] ) {
                cube( [ speaker_width, wall_thickness, speaker_depth ], true );
             }
        } else if ( which == 1 ) {
            translate( [ 0, -speaker_height, 0 ] ) {
                  cube( [ speaker_width, wall_thickness, speaker_depth ], true );
             }
        }   
}


draw_sides( -1 );
draw_sides( 1 );
draw_top( 1 );
draw_top( -1 );

