<?php

$E24 = [
    1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1
];

$E48 = [
1.00, 1.05, 1.10, 1.15, 1.21, 1.27, 1.33, 1.40, 1.47, 1.54, 1.62, 1.69, 1.78, 1.87, 1.96, 2.05, 2.15, 2.26, 2.37, 2.49, 2.61, 2.74, 2.87, 3.01, 3.16, 3.32, 3.48, 3.65, 3.83, 4.02, 4.22, 4.42, 4.64, 4.87, 5.11, 5.36, 5.62, 5.90, 6.19, 6.49, 6.81, 7.15, 7.50, 7.87, 8.25, 8.66, 9.09, 9.53
];

$r1 = 1100;
$r2 = 500;
$c1 = 100;
$c2 = 100;
$pi = 3.1415926535;

function compute_pole_zeros( $r1, $r2, $c1o, $c2o ) { 
    global $pi;

    $c1 = $c1o / 1000000000;
    $c2 = $c2o / 1000000000;

    $temp = $r2 * ( $c1 + $c2 );
    $a = $r1 * ( $c1 );
    $b = $temp + $a;
    $c = $r2 * $r1 * $c1 * $c2;

    $omega1 = -(-$b - sqrt( $b * $b - 4 * $c )) / ( 2 * $c );
    $omega2 = -(-$b + sqrt( $b * $b - 4 * $c )) / ( 2 * $c );

    $fz = 1/(2 * $pi * $r1 * $c1 );
    $fp1 = $omega1 / ( 2 * $pi );
    $fp2 = $omega2 / ( 2 * $pi );

    return [ 'zero' => $fz, 'pole1' => $fp1, 'pole2' => $fp2, 'a' => $a, 'b' => $b, 'c' => $c, 'temp' => $temp, 'r1' => $r1, 'r2' => $r2, 'c1' => $c1o, 'c2' => $c2o ];
}

$iterations = 250000;

function compute_score( &$results ) {
    $score = abs( $results[ 'zero' ] - 500 ) + abs ( $results[ 'pole1' ] - 2122 ) + abs ( $results[ 'pole2' ] - 50 );
    $results[ 'score' ] = $score;

    return $score;
}

function build_e_series( $set, $max, $total_combined = 2 ) {
    $new_set = [];
    $all_resistors = [];
    $mult = 1;
    for( $i = 0; $i < 4; $i++ ) {
        for ( $x = 0; $x < count( $set ); $x++ ) {
            $val = $set[ $x ] * $mult;
            if ( $val < $max ) {
                $all_resistors[] = $set[ $x ] * $mult;
            }
        }

        $mult = $mult*10;
    }

    if ( $total_combined == 2 ) {
        for ( $x = 0; $x < count( $all_resistors ); $x++ ) {
            for( $y = 0; $y < count( $all_resistors ); $y++ ) {
                $val = $all_resistors[ $x ] + $all_resistors[ $y ];
                if ( $val < $max ) {
                    $new_set[] = $val;
                }
            }
        }
    } else if ( $total_combined == 3 ) {
        for ( $x = 0; $x < count( $all_resistors ); $x++ ) {
            for( $y = 0; $y < count( $all_resistors ); $y++ ) {
                for( $z = 0; $z < count( $all_resistors ); $z++ ) {
                    $val = $all_resistors[ $x ] + $all_resistors[ $y ] + $all_resistors[ $z ];
                    if ( $val < $max ) {
                        $new_set[] = $val;
                    }
                }
            }
        }
    }


    sort( $new_set );

    return array_values( array_unique( $new_set ) );
}


//print_r( $capacitor_set );die;

function get_offset_in_series( $series, $value, $offset ) {
  ///  print_r( $series );
  //  echo $value; echo $offset;
    $pos = array_search( $value, $series );

    //echo "searching for " . $value . "\n";
    return $series[ $pos + $offset ];
}

function lock_c1( $set, $r1 ) {
    global $pi;
    $c1 = 0;

    $score = 100000;
    for( $i = 0; $i < count( $set ); $i++ ) {
        echo $set[ $i ]. "\n";
        $fz = 1/(2 * $pi * $r1 * $set[ $i ]*1e-9 );

        if ( abs( $fz - 500 ) < $score ) {
            $score = abs( $fz - 500 );
            $c1 =  $set[ $i ] ;
        }
    }

    return $c1;
}

$resistor_set = build_e_series( $E24, 8000, 3 );
$capacitor_set = build_e_series( $E24, 999, 2 );

$c1 = lock_c1( $capacitor_set, $r1 );

$results = compute_pole_zeros( $r1, $r2, $c1, $c2 );

$best_score = 100000;
$best_results = false;

for( $c = 0; $c < count( $capacitor_set); $c++ ) {
    $cap = $capacitor_set[ $c ];
    for( $r = 0; $r < count( $resistor_set ); $r++ ) {
        $res = $resistor_set[ $r ];

        $results = compute_pole_zeros( $r1, $res, $c1, $cap );
        $score = compute_score( $results );

        if ( $score < $best_score ) {
            $best_score = $score;
            $best_results = $results;
        }
    }
}

print_r( $best_results );



