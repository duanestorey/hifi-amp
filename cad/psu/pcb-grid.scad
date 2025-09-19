// Parametric PCB mounting plate with M3 clearance grid + central cutout + supports
// Units: millimeters

// ---------- Plate / Grid ----------
plate_len    = 215;         // X size (e.g., 210 mm = 21 cm)
plate_wid    = 215;         // Y size (e.g., 210 mm = 21 cm)
plate_thk    = 2;           // thickness

hole_d       = 3.4;         // M3 clearance
pitch_x      = 10;          // grid spacing X
pitch_y      = 10;          // grid spacing Y
edge_margin  = 5;           // minimum distance from edge to nearest hole

// ---------- Cutout (Toroid) ----------
cutout_on     = true;                        // enable/disable the cutout
cutout_d      = 98;                         // cutout diameter
cutout_center = [plate_len/2, plate_wid/2];  // [x, y] from plate's lower-left corner

// ---------- Supports (stand-offs) ----------
supports_on    = true;     // master toggle
support_d      = 5;        // diameter of cylindrical supports
support_h      = 10;       // height of supports
support_x      = 6;        // how many supports along X (cell centers)
support_y      = 6;        // how many supports along Y (cell centers)
supports_below = true;     // true: extend below the plate; false: above the plate

// ---------- Placement / Render ----------
center_plate  = true;      // center the whole plate at XY origin if true
$fn = 32;                  // smooth circles
eps = 0.02;                // tiny z-overshoot for clean through-cuts

// ---------- Helpers ----------
function _nsteps(L, margin, pitch) =
    max(1, floor((L - 2*margin) / pitch) + 1);

function _grid_positions(L, pitch, margin) =
    let(n = _nsteps(L, margin, pitch),
        span = (n - 1) * pitch,
        start = (L - span) / 2)                 // auto-center the grid
    [ for (i = [0:n-1]) start + i * pitch ];

function _cell_centers(pos) =
    [ for (i = [0:len(pos)-2]) (pos[i] + pos[i+1]) / 2 ];

function _evenly_spaced_indices(n, k) =
    (n <= 0) ? [] :
    (k <= 1) ? [floor((n-1)/2)] :
    (k >= n) ? [ for (i=[0:n-1]) i ] :
               [ for (i=[0:k-1]) floor(i * (n-1) / (k-1)) ];

function _cutout_z0(thk, sup_h, below) = below ? -sup_h - eps : -eps;
function _cutout_h(thk, sup_h, below)  = below ? (thk + sup_h + 2*eps)
                                               : (thk + sup_h + 2*eps);

// ---------- Core ----------
module _plate_body(len, wid, thk, hd, px, py, m,
                   cut_en, cut_d, cut_ctr,
                   sup_on, sup_d, sup_h, sup_nx, sup_ny, sup_below) {

    // Precompute hole grid
    xs = _grid_positions(len, px, m);
    ys = _grid_positions(wid, py, m);

    // Base + supports (union), then subtract holes and cutout
    difference() {
        // --- SOLIDS: base sheet + supports ---
        union() {
            // Base sheet
            cube([len, wid, thk], center = false);

            // Supports at cell-centers between holes (non-interfering with holes)
            if (sup_on) {
                cx = _cell_centers(xs);
                cy = _cell_centers(ys);

                idx_x = _evenly_spaced_indices(len(cx), sup_nx);
                idx_y = _evenly_spaced_indices(len(cy), sup_ny);

                // Support Z placement (below or above)
                z0 = sup_below ? -sup_h : thk;

                for (ix = idx_x)
                for (iy = idx_y)
                    translate([cx[ix], cy[iy], z0])
                        cylinder(h = sup_h, d = sup_d);
            }
        }

        // --- SUBTRACT: grid holes + (deeper) toroid cutout ---
        union() {
            // Through-holes (only need to clear the plate thickness)
            for (x = xs)
            for (y = ys)
                translate([x, y, -eps])
                    cylinder(h = thk + 2*eps, d = hd);

            // Cutout clears plate AND any supports in its way
            if (cut_en) {
                translate([cut_ctr[0], cut_ctr[1], _cutout_z0(thk, sup_h, sup_below)])
                    cylinder(h = _cutout_h(thk, sup_h, sup_below), d = cut_d);
            }
        }
    }
}

module mounting_plate(len=plate_len, wid=plate_wid, thk=plate_thk,
                      hd=hole_d, px=pitch_x, py=pitch_y, m=edge_margin,
                      cut_en=cutout_on, cut_d=cutout_d, cut_ctr=cutout_center,
                      sup_on=supports_on, sup_d=support_d, sup_h=support_h,
                      sup_nx=support_x, sup_ny=support_y, sup_below=supports_below,
                      centered=center_plate) {
    if (centered)
        translate([-len/2, -wid/2, 0])
            _plate_body(len, wid, thk, hd, px, py, m,
                        cut_en, cut_d, cut_ctr,
                        sup_on, sup_d, sup_h, sup_nx, sup_ny, sup_below);
    else
        _plate_body(len, wid, thk, hd, px, py, m,
                    cut_en, cut_d, cut_ctr,
                    sup_on, sup_d, sup_h, sup_nx, sup_ny, sup_below);
}

// ---------- Render ----------
mounting_plate();