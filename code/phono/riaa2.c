// riaa.c
// compile: gcc -O3 -std=c11 -lm -o riaa riaa.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI 3.1415926535

// ---------------- Base series ----------------
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

// ------------- Composition tracking -------------
typedef struct {
    int n;       // number of parts 1..3
    double p[3]; // parts (already decade-scaled)
} Parts;

// Current build’s parts array (parallel to new_set values).
static Parts *current_parts = NULL;
static size_t current_parts_len = 0;
static size_t current_parts_cap = 0;

static void parts_store(int n, double p0, double p1, double p2) {
    if (current_parts_len == current_parts_cap) {
        current_parts_cap = current_parts_cap ? current_parts_cap * 2 : 2048;
        current_parts = (Parts *)realloc(current_parts, current_parts_cap * sizeof(Parts));
        if (!current_parts) { fprintf(stderr, "OOM\n"); exit(1); }
    }
    current_parts[current_parts_len].n    = n;
    current_parts[current_parts_len].p[0] = p0;
    current_parts[current_parts_len].p[1] = p1;
    current_parts[current_parts_len].p[2] = p2;
    current_parts_len++;
}

// Saved arrays for resistor and capacitor sets
static Parts *res_parts = NULL;   static size_t res_parts_len = 0;
static Parts *cap_parts = NULL;   static size_t cap_parts_len = 0;

// ---------------- Your math (unchanged) ----------------
typedef struct {
    double zero, pole1, pole2;
    double a, b, c, temp;
    double r1, r2, c1, c2;
    double score;
} Results;

static Results compute_pole_zeros(double r1, double r2, double c1o, double c2o) {
    double c1 = c1o / 1e9;
    double c2 = c2o / 1e9;

    double temp = r2 * (c1 + c2);
    double a = r1 * (c1);
    double b = temp + a;
    double c = r2 * r1 * c1 * c2;

    double disc = b*b - 4.0*c;
    if (disc < 0.0) disc = 0.0;

    double omega1 = -(-b - sqrt(disc)) / (2.0 * c);
    double omega2 = -(-b + sqrt(disc)) / (2.0 * c);

    double fz  = 1.0 / (2.0 * PI * r1 * c1);
    double fp1 = omega1 / (2.0 * PI);
    double fp2 = omega2 / (2.0 * PI);

    Results R;
    R.zero=fz; R.pole1=fp1; R.pole2=fp2;
    R.a=a; R.b=b; R.c=c; R.temp=temp;
    R.r1=r1; R.r2=r2; R.c1=c1o; R.c2=c2o; R.score=0.0;
    return R;
}

static double compute_score(Results *results) {
    double score = fabs(results->zero - 500.0)
                 + fabs(results->pole1 - 2122.0)
                 + fabs(results->pole2 - 50.0);
    results->score = score;
    return score;
}

// ---------- small helper to push value + parts (C-friendly) ----------
static void push_value(double **arr, size_t *len, size_t *cap,
                       double v, int n, double p0, double p1, double p2) {
    if (*len == *cap) {
        *cap = *cap ? (*cap * 2) : 512;
        *arr = (double*)realloc(*arr, (*cap) * sizeof(double));
        if (!*arr) { fprintf(stderr, "OOM\n"); exit(1); }
    }
    (*arr)[(*len)++] = v;
    parts_store(n, p0, p1, p2);
}

// ---------------- Your builder (same behavior), plus recipes ----------------
static double* build_e_series(const double *set, size_t set_len, double max, int total_combined, size_t *out_len) {
    double *new_set = NULL;
    size_t len = 0, cap = 0;

    double *all_vals = NULL;
    size_t all_len = 0, all_cap = 0;

    // Build all_resistors style: 4 decades (x1, x10, x100, x1000)
    double mult = 1.0;
    for (int i = 0; i < 4; i++) {
        for (size_t x = 0; x < set_len; x++) {
            double val = set[x] * mult;
            if (val < max) {
                if (all_len == all_cap) {
                    all_cap = all_cap ? all_cap * 2 : 256;
                    all_vals = (double*)realloc(all_vals, all_cap * sizeof(double));
                    if (!all_vals) { fprintf(stderr, "OOM\n"); exit(1); }
                }
                all_vals[all_len++] = val;
            }
        }
        mult *= 10.0;
    }

    // IMPORTANT: do NOT free current_parts here; caller resets it before calling us.
    // Just ensure its counters start at 0 for this build:
    current_parts_len = 0;
    // (capacity and pointer are managed by parts_store)

    // Singles
    for (size_t x = 0; x < all_len; x++) {
        double v = all_vals[x];
        if (v < max) push_value(&new_set, &len, &cap, v, 1, all_vals[x], 0.0, 0.0);
    }

    // Two-part sums
    if (total_combined >= 2) {
        for (size_t x = 0; x < all_len; x++) {
            for (size_t y = 0; y < all_len; y++) {
                double v = all_vals[x] + all_vals[y];
                if (v < max) push_value(&new_set, &len, &cap, v, 2, all_vals[x], all_vals[y], 0.0);
            }
        }
    }

    // Three-part sums
    if (total_combined >= 3) {
        for (size_t x = 0; x < all_len; x++) {
            for (size_t y = 0; y < all_len; y++) {
                for (size_t z = 0; z < all_len; z++) {
                    double v = all_vals[x] + all_vals[y] + all_vals[z];
                    if (v < max) push_value(&new_set, &len, &cap, v, 3, all_vals[x], all_vals[y], all_vals[z]);
                }
            }
        }
    }

    // Sort new_set and keep parts aligned (selection sort for simplicity)
    for (size_t i = 0; i + 1 < len; i++) {
        size_t m = i;
        for (size_t j = i + 1; j < len; j++) {
            if (new_set[j] < new_set[m]) m = j;
        }
        if (m != i) {
            double tv = new_set[i];
            new_set[i] = new_set[m];
            new_set[m] = tv;

            Parts tp = current_parts[i];
            current_parts[i] = current_parts[m];
            current_parts[m] = tp;
        }
    }

    // Unique (exact equality)
    size_t w = 0;
    for (size_t r = 0; r < len; r++) {
        if (w == 0 || new_set[r] != new_set[w - 1]) {
            new_set[w] = new_set[r];
            current_parts[w] = current_parts[r];
            w++;
        }
    }
    len = w;

    free(all_vals);
    *out_len = len;
    return new_set;
}

// Lock C1 to nearest zero; return index (so we can print the combo)
static size_t lock_c1_idx(const double *series, size_t n, double r1, const Parts *cparts) {
    (void)cparts;
    double best_err = 1e99;
    size_t best_i = 0;
    for (size_t i = 0; i < n; i++) {
        double fz = 1.0 / (2.0 * PI * r1 * (series[i] * 1e-9));
        double err = fabs(fz - 500.0);
        if (err < best_err) {
            best_err = err;
            best_i = i;
        }
    }
    return best_i;
}

int main(void) {
    // Choose which base to use (same for R and C here)
    const double *BASE = E24; size_t BASE_LEN = E24_LEN;
    // const double *BASE = E96; size_t BASE_LEN = E96_LEN;
    // const double *BASE = E192; size_t BASE_LEN = E192_LEN;

    // Limits like your PHP
    double r1 = 1100.0;

    // ---- Build resistor set: max 8000, up to 3 combined ----
    size_t resistor_len = 0;
    current_parts = NULL; current_parts_len = current_parts_cap = 0;
    double *resistor_set = build_e_series(BASE, BASE_LEN, 8000.0, 3, &resistor_len);
    res_parts = current_parts; res_parts_len = current_parts_len;

    // ---- Build capacitor set: max 999 nF, up to 2 combined ----
    size_t capacitor_len = 0;
    current_parts = NULL; current_parts_len = current_parts_cap = 0;
    double *capacitor_set = build_e_series(BASE, BASE_LEN, 999.0, 2, &capacitor_len);
    cap_parts = current_parts; cap_parts_len = current_parts_len;

    fprintf(stderr, "R choices: %zu | C choices: %zu\n", resistor_len, capacitor_len);

    // Lock C1 to zero target
    size_t c1_idx = lock_c1_idx(capacitor_set, capacitor_len, r1, cap_parts);
    double c1_nf = capacitor_set[c1_idx];

    double best_score = 1e99;
    int have_best = 0;
    size_t best_r_idx = 0, best_c_idx = 0;

    // Brute-force search over (R2, C2)
    for (size_t ci = 0; ci < capacitor_len; ci++) {
        double cap_nf = capacitor_set[ci];
        for (size_t ri = 0; ri < resistor_len; ri++) {
            double res_ohm = resistor_set[ri];

            Results res = compute_pole_zeros(r1, res_ohm, c1_nf, cap_nf);
            double sc = compute_score(&res);

            if (sc < best_score) {
                best_score = sc;
                have_best = 1;
                best_r_idx = ri;
                best_c_idx = ci;
            }
        }
    }

    if (!have_best) {
        printf("No best result found.\n");
        free(resistor_set); free(res_parts);
        free(capacitor_set); free(cap_parts);
        return 0;
    }

    // Final best results
    double best_r2 = resistor_set[best_r_idx];
    double best_c2 = capacitor_set[best_c_idx];

    Results best = compute_pole_zeros(r1, best_r2, c1_nf, best_c2);
    compute_score(&best);

    // -------- Clean C-style report --------
    printf("Best solution:\n");
    printf("  zero  = %.10f Hz\n", best.zero);
    printf("  pole1 = %.10f Hz\n", best.pole1);
    printf("  pole2 = %.10f Hz\n", best.pole2);
    printf("  a     = %.10f\n",  best.a);
    printf("  b     = %.10f\n",  best.b);
    printf("  c     = %.10f\n",  best.c);
    printf("  temp  = %.10f\n",  best.temp);
    printf("  R1    = %.2f ohm\n", best.r1);
    printf("  R2    = %.2f ohm\n", best.r2);
    printf("  C1    = %.2f nF\n",  best.c1);
    printf("  C2    = %.2f nF\n",  best.c2);
    printf("  score = %.10f\n",  best.score);

    // -------- Compositions (the new bit) --------
    printf("\nCompositions:\n");

    Parts rp = res_parts[best_r_idx];
    printf("  R2 = %.2f ohm = ", best_r2);
    for (int k = 0; k < rp.n; k++) {
        if (k) printf(" + ");
        printf("%.2f", rp.p[k]);
    }
    printf("\n");

    Parts cp1 = cap_parts[c1_idx];
    printf("  C1 = %.2f nF  = ", c1_nf);
    for (int k = 0; k < cp1.n; k++) {
        if (k) printf(" + ");
        printf("%.2f", cp1.p[k]);
    }
    printf("\n");

    Parts cp2 = cap_parts[best_c_idx];
    printf("  C2 = %.2f nF  = ", best_c2);
    for (int k = 0; k < cp2.n; k++) {
        if (k) printf(" + ");
        printf("%.2f", cp2.p[k]);
    }
    printf("\n");

    // Cleanup
    free(resistor_set);
    free(capacitor_set);
    free(res_parts);
    free(cap_parts);
    return 0;
}