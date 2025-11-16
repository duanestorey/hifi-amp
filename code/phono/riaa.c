// compile: gcc -O3 -std=c11 -lm -o riaa riaa.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.1415926535

typedef struct {
    double zero, pole1, pole2;
    double a, b, c, temp;
    double r1, r2, c1, c2;
    double score;
    int components;
} Results;

typedef struct {
    double *data;
    double *v1;
    double *v2;
    double *v3;

    size_t len;
    size_t cap;
} Vec;

static void vec_init(Vec *v) { v->data = v->v1 = v->v2 = v->v3 = NULL; v->len = 0; v->cap = 0; }
static void vec_free(Vec *v) { free(v->data); free(v->v1); free( v->v2); free (v->v3); v->data = v->v1 = v->v2 = v->v3 = NULL; v->len = v->cap = 0; }

static void vec_push(Vec *v, double x, double v1, double v2, double v3 ) {
    if (v->len == v->cap) {
        size_t ncap = v->cap ? v->cap * 2 : 4096;
        double *nd = (double *)realloc(v->data, ncap * sizeof(double));
        if (!nd) { fprintf(stderr, "OOM\n"); exit(1); }
        v->data = nd; v->cap = ncap;

        v->v1 = (double *)realloc(v->v1, ncap * sizeof(double));
        v->v2 = (double *)realloc(v->v2, ncap * sizeof(double));
        v->v3 = (double *)realloc(v->v3, ncap * sizeof(double));
    }
    v->data[v->len] = x;

  //  printf( "Adding %.10f\n", x );
    v->v1[v->len] = v1;
    v->v2[v->len] = v2;
    v->v3[v->len] = v3;

    v->len++;
}

static int dbl_cmp(const void *a, const void *b) {
    double da = *(const double*)a, db = *(const double*)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

// ---- Data ----
static const double MOUSER[] = {
    //1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.7, 3.0, 3.3, 3.9, 4.3, 4.7, 5.1, 5.6, 6.0, 6.8, 8.2
    //1.0, 1.5, 2.2, 2.4, 2.7, 3.3, 3.6, 3.9, 4.7, 5.6, 6.8, 8.2
    1.0, 2.2, 4.7
};

static const size_t MOUSER_LEN = sizeof(MOUSER)/sizeof(MOUSER[0]);

static const double E24[] = {
    1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0,
    3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1
};
static const size_t E24_LEN = sizeof(E24)/sizeof(E24[0]);

static const double E48[] = {
    1.00, 1.05, 1.10, 1.15, 1.21, 1.27, 1.33, 1.40, 1.47, 1.54, 1.62, 1.69,
    1.78, 1.87, 1.96, 2.05, 2.15, 2.26, 2.37, 2.49, 2.61, 2.74, 2.87, 3.01,
    3.16, 3.32, 3.48, 3.65, 3.83, 4.02, 4.22, 4.42, 4.64, 4.87, 5.11, 5.36,
    5.62, 5.90, 6.19, 6.49, 6.81, 7.15, 7.50, 7.87, 8.25, 8.66, 9.09, 9.53
};
static const size_t E48_LEN = sizeof(E48)/sizeof(E48[0]);

// E96 preferred numbers (IEC 60063) — base decade 1.00..9.76 (96 values)
static const double E96[] = {
    1.00, 1.02, 1.05, 1.07, 1.10, 1.13, 1.15, 1.18, 1.21, 1.24, 1.27, 1.30,
    1.33, 1.37, 1.40, 1.43, 1.47, 1.50, 1.54, 1.58, 1.62, 1.65, 1.69, 1.74,
    1.78, 1.82, 1.87, 1.91, 1.96, 2.00, 2.05, 2.10, 2.15, 2.21, 2.26, 2.32,
    2.37, 2.43, 2.49, 2.55, 2.61, 2.67, 2.74, 2.80, 2.87, 2.94, 3.01, 3.09,
    3.16, 3.24, 3.32, 3.40, 3.48, 3.57, 3.65, 3.74, 3.83, 3.92, 4.02, 4.12,
    4.22, 4.32, 4.42, 4.53, 4.64, 4.75, 4.87, 4.99, 5.11, 5.23, 5.36, 5.49,
    5.62, 5.76, 5.90, 6.04, 6.19, 6.34, 6.49, 6.65, 6.81, 6.98, 7.15, 7.32,
    7.50, 7.68, 7.87, 8.06, 8.25, 8.45, 8.66, 8.87, 9.09, 9.31, 9.53, 9.76
};
static const size_t E96_LEN = sizeof(E96)/sizeof(E96[0]);

// E192 preferred numbers (IEC 60063) — base decade 1.00..9.76 (192 values)
static const double E192[] = {
    1.00, 1.01, 1.02, 1.04, 1.05, 1.06, 1.07, 1.09, 1.10, 1.11, 1.13, 1.14,
    1.15, 1.17, 1.18, 1.20, 1.21, 1.23, 1.24, 1.26, 1.27, 1.29, 1.30, 1.32,
    1.33, 1.35, 1.37, 1.38, 1.40, 1.42, 1.43, 1.45, 1.47, 1.49, 1.50, 1.52,
    1.54, 1.56, 1.58, 1.60, 1.62, 1.64, 1.65, 1.67, 1.69, 1.72, 1.74, 1.76,
    1.78, 1.80, 1.82, 1.84, 1.87, 1.89, 1.91, 1.93, 1.96, 1.98, 2.00, 2.03,
    2.05, 2.08, 2.10, 2.13, 2.15, 2.18, 2.21, 2.23, 2.26, 2.29, 2.32, 2.34,
    2.37, 2.40, 2.43, 2.46, 2.49, 2.52, 2.55, 2.58, 2.61, 2.64, 2.67, 2.71,
    2.74, 2.77, 2.80, 2.84, 2.87, 2.91, 2.94, 2.98, 3.01, 3.05, 3.09, 3.12,
    3.16, 3.20, 3.24, 3.28, 3.32, 3.36, 3.40, 3.44, 3.48, 3.52, 3.57, 3.61,
    3.65, 3.70, 3.74, 3.79, 3.83, 3.88, 3.92, 3.97, 4.02, 4.07, 4.12, 4.17,
    4.22, 4.27, 4.32, 4.37, 4.42, 4.48, 4.53, 4.59, 4.64, 4.70, 4.75, 4.81,
    4.87, 4.93, 4.99, 5.05, 5.11, 5.17, 5.23, 5.30, 5.36, 5.42, 5.49, 5.56,
    5.62, 5.69, 5.76, 5.83, 5.90, 5.97, 6.04, 6.12, 6.19, 6.26, 6.34, 6.42,
    6.49, 6.57, 6.65, 6.73, 6.81, 6.90, 6.98, 7.06, 7.15, 7.23, 7.32, 7.41,
    7.50, 7.59, 7.68, 7.77, 7.87, 7.96, 8.06, 8.16, 8.25, 8.35, 8.45, 8.56,
    8.66, 8.76, 8.87, 8.98, 9.09, 9.20, 9.31, 9.42, 9.53, 9.65, 9.76
};
static const size_t E192_LEN = sizeof(E192)/sizeof(E192[0]);

// ---- Core math ----
static Results compute_pole_zeros(double r1, double r2, double c1o, double c2o) {
    // c1o, c2o are in nF
    double c1 = c1o / 1000000000.0;
    double c2 = c2o / 1000000000.0;

    double temp = r2 * (c1 + c2);
    double a = r1 * (c1);
    double b = temp + a;
    double c = r2 * r1 * c1 * c2;

    double disc = b * b - 4.0 * c;
    double omega1 = -(-b - sqrt(disc)) / (2.0 * c);
    double omega2 = -(-b + sqrt(disc)) / (2.0 * c);

    double fz  = 1.0 / (2.0 * PI * r1 * c1);
    double fp1 = omega1 / (2.0 * PI);
    double fp2 = omega2 / (2.0 * PI);

    Results R;
    R.zero = fz; R.pole1 = fp1; R.pole2 = fp2;
    R.a = a; R.b = b; R.c = c; R.temp = temp;
    R.r1 = r1; R.r2 = r2; R.c1 = c1o; R.c2 = c2o;
    R.score = 0.0;

    return R;
}

static double compute_score(Results *res) {
    double score = fabs(res->zero - 500.0)/500.0 + fabs(res->pole1 - 2122.0)/2122.0 + fabs(res->pole2 - 50.0)/50.0;
    res->score = score;
    return score;
}

// Put above build_e_series
static inline void vec_swap_rows(Vec *v, size_t i, size_t j) {
    double td = v->data[i]; v->data[i] = v->data[j]; v->data[j] = td;
    double t1 = v->v1[i];   v->v1[i]   = v->v1[j];   v->v1[j]   = t1;
    double t2 = v->v2[i];   v->v2[i]   = v->v2[j];   v->v2[j]   = t2;
    double t3 = v->v3[i];   v->v3[i]   = v->v3[j];   v->v3[j]   = t3;
}

static void sort_vec_by_data(Vec *v) {
    // Simple insertion sort (stable) — plenty fast for these list sizes
    for (size_t i = 1; i < v->len; i++) {
        size_t j = i;
        while (j > 0 && v->data[j-1] > v->data[j]) {
            vec_swap_rows(v, j-1, j);
            j--;
        }
    }
}

// ---- Library build (exactly like your PHP) ----
static void build_e_series(const double *set, size_t set_len, double min_val, double max_val,
                           int total_combined, Vec *out_new_set)
{
    Vec all_resistors; vec_init(&all_resistors);
    // four decades: *1, *10, *100, *1000
    double mult = 1.0;
    for (int i = 0; i < 5; i++) {
        for (size_t x = 0; x < set_len; x++) {
            double val = set[x] * mult;
            if (val > min_val && val < max_val) {
                vec_push(&all_resistors, val, val, val, val);
            }
        }
        mult *= 10.0;
    }

    Vec new_set; vec_init(&new_set);

    if (total_combined == 1) {
        for (size_t y = 0; y < all_resistors.len; y++) {
            double val = all_resistors.data[y];
            if (val < max_val) vec_push(&new_set, val, all_resistors.data[y], 0, 0);
        }
    } else if (total_combined == 2) {
        for (size_t x = 0; x < all_resistors.len; x++) {
            vec_push(&new_set, all_resistors.data[x], all_resistors.data[x], 0, 0);
            for (size_t y = 0; y < all_resistors.len; y++) {
                double val = all_resistors.data[x] + all_resistors.data[y];
                if (val < max_val*2) vec_push(&new_set, val, all_resistors.data[x], all_resistors.data[y], 0);
            }
        }
    } else if (total_combined == 3) {
        for (size_t x = 0; x < all_resistors.len; x++) {
            vec_push(&new_set, all_resistors.data[x], all_resistors.data[x], 0, 0);
            for (size_t y = 0; y < all_resistors.len; y++) {
                vec_push(&new_set, all_resistors.data[x] + all_resistors.data[y], all_resistors.data[x],  all_resistors.data[y], 0);
                for (size_t z = 0; z < all_resistors.len; z++) {
                    double val = all_resistors.data[x] + all_resistors.data[y] + all_resistors.data[z];
                    if (val < max_val*3) vec_push(&new_set, val, all_resistors.data[x], all_resistors.data[y], all_resistors.data[z]);
                }
            }
        }
    }

    // sort
    //qsort(new_set.data, new_set.len, sizeof(double), dbl_cmp);
    sort_vec_by_data(&new_set);

    // unique (exact equality, like PHP array_unique() with floats)
    Vec uniq; vec_init(&uniq);
    if (new_set.len > 0) {
        vec_push(&uniq, new_set.data[0], new_set.v1[0], new_set.v2[0], new_set.v3[0]);
        for (size_t i = 1; i < new_set.len; i++) {
            //printf( "%.10f\n", new_set.data[i] );
            if (new_set.data[i] != new_set.data[i-1]) {
                vec_push(&uniq, new_set.data[i], new_set.v1[i], new_set.v2[i], new_set.v3[i]);
               // printf( "%.10f\n", new_set.data[i] );
            }
        }
    }

    // move uniq into out_new_set
    *out_new_set = uniq;
    vec_free(&new_set);
    vec_free(&all_resistors);
}

// ---- get_offset_in_series (unused in your brute-force, but included verbatim) ----
static double get_offset_in_series(const double *series, size_t n, double value, int offset) {
    // array_search exact
    long pos = -1;
    for (size_t i = 0; i < n; i++) {
        if (series[i] == value) { pos = (long)i; break; }
    }
    // no bounds checks (mirrors your PHP; may crash if misused)
    return series[pos + offset];
}

// ---- lock_c1 (prints each candidate like your PHP) ----
static double lock_c1(const double *set, size_t n, double r1) {
    double c1 = 0.0;
    double best = 100000.0;
    for (size_t i = 0; i < n; i++) {
    //    printf("%g\n", set[i]); // echo each candidate like PHP
        double fz = 1.0 / (2.0 * PI * r1 * set[i] * 1e-9);
        double err = fabs(fz - 500.0);
        if (err < best) {
            best = err;
            c1 = set[i];
        }
    }
    return c1;
}

static double compute_for_r1( double r1, double r2, double c1, double c2, Vec *capacitor_set, Vec *resistor_set, Results *bestResults ) {
    // initial compute (same as PHP)
    Results results = compute_pole_zeros(r1, r2, c1, c2);

    double best_score = 100000.0;
    Results best_results;
    int have_best = 0;
    int best_components = 0;

    // brute-force search
    for (size_t ci = 0; ci < capacitor_set->len; ci++) {
        double cap = capacitor_set->data[ci];
        for (size_t ri = 0; ri < resistor_set->len; ri++) {
            double res = resistor_set->data[ri];
            int components = ((int)( capacitor_set->v1[ci] > 0 )) +  ((int)( capacitor_set->v2[ci] > 0 )) +  ((int)( capacitor_set->v3[ci] > 0 )) + 
                ((int)( resistor_set->v1[ri] > 0 )) +  ((int)( resistor_set->v2[ri] > 0 )) +  ((int)( resistor_set->v3[ri] > 0 ));

            Results R = compute_pole_zeros(r1, res, c1, cap);
            double sc = compute_score(&R);

            if (sc < best_score) {
                best_score = sc;
                best_results = R;
                have_best = 1;
                best_components = components;
            }
        }
    }

    if (have_best) {
        memcpy( bestResults, &best_results, sizeof( Results ));
        // print_r style dump (simple)
        /*
        printf("Array\n(\n");
        printf("    [zero] => %.10f\n", best_results.zero);
        printf("    [pole1] => %.10f\n", best_results.pole1);
        printf("    [pole2] => %.10f\n", best_results.pole2);
        printf("    [a] => %.10f\n", best_results.a);
        printf("    [b] => %.10f\n", best_results.b);
        printf("    [c] => %.10f\n", best_results.c);
        printf("    [temp] => %.10f\n", best_results.temp);
        printf("    [r1] => %.10f\n", best_results.r1);
        printf("    [r2] => %.10f\n", best_results.r2);
        printf("    [c1] => %.10f\n", best_results.c1);
        printf("    [c2] => %.10f\n", best_results.c2);
        printf("    [score] => %.10f\n", best_results.score);
        printf(")\n");
        */
        if ( best_results.score < 1 ) {
            printf( "[score] %.10f [zero] %.10f [pole1] %.10f [pole2] %.10f [r1] %.10f [r2] %.10f [c1] %.10f [c2] %.10f [comp] %d\n", best_results.score, best_results.zero, best_results.pole1, best_results.pole2, best_results.r1, best_results.r2, best_results.c1, best_results.c2, best_components );
        }
        return best_results.score;
    } else {
        printf("No best result found.\n");
        return 0;
    }
}

int main(void) {
    // initial values
    double r1 = 1000.0;
    double r2 = 500.0;
    double c1 = 100.0;
    double c2 = 100.0;
    double bestScore = 100000;

    Results bestResults;

    // build libraries (exact params as your PHP)
    Vec resistor_set; vec_init(&resistor_set);
    Vec capacitor_set; vec_init(&capacitor_set);

    build_e_series(E48, E48_LEN, 100, 20000.0, 2, &resistor_set);  // up to 3 combined
    build_e_series(MOUSER, MOUSER_LEN,  1, 150.0, 3, &capacitor_set); // up to 2 combined

    for( int i = 0; i < resistor_set.len; i++ ) {
        if ( resistor_set.data[ i ] < 1000 || resistor_set.data[ i ] > 6000 ) continue;

        // lock C1
        c1 = lock_c1(capacitor_set.data, capacitor_set.len, resistor_set.data[ i ] );

        Results tempResults;
        double score = compute_for_r1( resistor_set.data[ i ], r2, c1, c2, &capacitor_set, &resistor_set, &tempResults );  

        if ( score < bestScore ) {
            bestScore = score;
            bestResults = tempResults;
        }
    }

    printf( "\nBest Score was:\n[score] %.10f [r1] %.10f [r2] %.10f [c1] %.10f [c2] %.10f\n", bestResults.score, bestResults.r1, bestResults.r2, bestResults.c1, bestResults.c2 );

    Results R = compute_pole_zeros(bestResults.r1, bestResults.r2, bestResults.c1, bestResults.c2);
    printf( "[zero] %.10f [pole1] %.10f [pole2] %.10f\n", R.zero, R.pole1, R.pole2 );
      
    for( int i = 0; i < resistor_set.len; i++ ) {
        if ( resistor_set.data[ i ] == bestResults.r1 ) {
            printf( "\tr1 values were %.10f %.10f %.10f\n",resistor_set.v1[i], resistor_set.v2[i], resistor_set.v3[i] );
        }

        if ( resistor_set.data[ i ] == bestResults.r2) {
            printf( "\tr2 values were %.10f %.10f %.10f\n",resistor_set.v1[i], resistor_set.v2[i], resistor_set.v3[i] );
        }
    }

    for( int i = 0; i < capacitor_set.len; i++ ) {
        if ( capacitor_set.data[ i ] == bestResults.c1 ) {
            printf( "\tc1 values were %.10f %.10f %.10f\n",capacitor_set.v1[i], capacitor_set.v2[i], capacitor_set.v3[i] );
        }

        if ( capacitor_set.data[ i ] == bestResults.c2) {
            printf( "\tc2 values were %.10f %.10f %.10f\n",capacitor_set.v1[i], capacitor_set.v2[i], capacitor_set.v3[i] );
        }
    }
    //compute_for_r1( r1, r2, c1, c2, &capacitor_set, &resistor_set );

    vec_free(&resistor_set);
    vec_free(&capacitor_set);
    return 0;
}
