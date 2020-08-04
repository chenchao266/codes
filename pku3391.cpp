#include <cstdio> 
#include <cmath> 
#include <cstring> 
#include <cstdlib> 
#include <algorithm> 

using namespace std; 

#define TRUE 1 
#define FALSE 0 
#define Vector(p1, p2, u, v) (u = p2->x - p1->x, v = p2->y - p1->y) 
#define Cross_product_2v(u1, v1, u2, v2) (u1 * v2 - v1 * u2) 
#define Cross_product_3p(p1, p2, p3) ((p2->x - p1->x) * (p3->y - p1->y) - (p2->y - p1->y) * (p3->x - p1->x)) 
#define Dot_product_2v(u1, v1, u2, v2) (u1 * u2 + v1 * v2) 
#define Org(e) ((e)->org) 
#define Dest(e) ((e)->dest) 
#define Onext(e) ((e)->onext) 
#define Oprev(e) ((e)->oprev) 
#define Dnext(e) ((e)->dnext) 
#define Dprev(e) ((e)->dprev) 
#define Other_point(e, p) ((e)->org == p ? (e)->dest : (e)->org) 
#define Next(e, p) ((e)->org == p ? (e)->onext : (e)->dnext) 
#define Prev(e, p) ((e)->org == p ? (e)->oprev : (e)->dprev) 
#define Visited(p) ((p)->f) 
#define Identical_refs(e1, e2) (e1 == e2) 

typedef enum {right, left} side; 

typedef double Real; 
typedef int boolean; 

typedef struct point point; 
typedef struct edge edge; 

struct point 
{ 
    Real x, y; 
    edge *entry_pt; 
}; 

struct edge 
{ 
    point *org, *dest; 
    edge *onext, *oprev, *dnext, *dprev; 
}; 

#define MAXV 1000001 
#define MAXE 3000001 

struct EDGE 
{ 
    int u, v; 
    double w; 
}; 

EDGE E[MAXE], mst[MAXE]; 

int n, en, k;
double dist[MAXV];

int p[MAXV], d[MAXV]; 

void UFinit(int m) 
{ 
    int i; 
    for(i = 0; i < m; i++) 
    { 
        p[i] = i, d[i] = 0; 
    } 
} 

int UFfind(int x) 
{ 
    int i = x, j, k; 
    while(p[i] != i) i = p[i]; 
    j = x; 
    while(p[j] != i) 
    { 
        k = p[j]; 
        p[j] = i; 
        j = k; 
    } 
    return i; 
} 

void UFunion(int x, int y) 
{ 
    int i = UFfind(x), j = UFfind(y); 
    if(d[i] > d[j]) p[j] = i; 
    else 
    { 
        p[i] = j; 
        if(d[i] == d[j]) d[j]++; 
    } 
} 

bool cmp(const EDGE &e1, const EDGE &e2) 
{ 
    return e1.w < e2.w; 
} 

void kruskal(void)
{
	if(k > n) k = n;
	int i, kk;
	sort(E, E + en, cmp);
	UFinit(n);
	for(i = 0, kk = 0; i < en && kk < n - 1; i++)
		if(UFfind(E[i].u) != UFfind(E[i].v))
		{
			UFunion(E[i].u, E[i].v);
			mst[kk++] = E[i];
		}
	printf("%d\n", int(ceil(mst[n - k - 1].w) + 0.00000001));
}

point *p_array; 
static edge *e_array; 
static edge **free_list_e; 
static int n_free_e; 

void alloc_memory(int n) 
{ 
    edge *e; 
    int i; 
    p_array = (point *)calloc(n, sizeof(point)); 
    n_free_e = 3 * n; 
    e_array = e = (edge *)calloc(n_free_e, sizeof(edge)); 
    free_list_e = (edge **)calloc(n_free_e, sizeof(edge *)); 
    for(i = 0; i < n_free_e; i++, e++) 
        free_list_e[i] = e; 
} 

void free_memory(void) 
{ 
    free(p_array);   
    free(e_array); 
    free(free_list_e);   
} 

edge *get_edge(void) 
{ 
    if(n_free_e == 0) 
        printf("Out of memory for edges\n"); 
    return (free_list_e[--n_free_e]); 
} 

void free_edge(edge *e) 
{ 
    free_list_e[n_free_e++] = e; 
} 

void splice(edge *a, edge *b, point *v) 
{ 
    edge *next; 
    if(Org(a) == v) 
    {  
        next = Onext(a); 
        Onext(a) = b; 
    } 
    else 
    { 
        next = Dnext(a); 
        Dnext(a) = b; 
    } 
    if(Org(next) == v) 
        Oprev(next) = b; 
    else 
        Dprev(next) = b; 
    if(Org(b) == v) 
    { 
        Onext(b) = next; 
        Oprev(b) = a; 
    } 
    else 
    { 
        Dnext(b) = next; 
        Dprev(b) = a; 
    } 
} 

edge *make_edge(point *u, point *v) 
{ 
    edge *e; 
    e = get_edge(); 
    e->onext = e->oprev = e->dnext = e->dprev = e; 
    e->org = u; 
    e->dest = v; 
    if(u->entry_pt == NULL) 
        u->entry_pt = e; 
    if(v->entry_pt == NULL) 
        v->entry_pt = e; 
    return e; 
} 

edge *join(edge *a, point *u, edge *b, point *v, side s) 
{ 
    edge *e; 
    e = make_edge(u, v); 
    if(s == left) 
    { 
        if (Org(a) == u) 
            splice(Oprev(a), e, u); 
        else 
            splice(Dprev(a), e, u); 
        splice(b, e, v); 
    } 
    else 
    { 
        splice(a, e, u); 
        if(Org(b) == v) 
            splice(Oprev(b), e, v); 
        else 
            splice(Dprev(b), e, v); 
    } 
    return e; 
} 

void delete_edge(edge *e) 
{ 
    point *u, *v; 
    u = Org(e); 
    v = Dest(e); 
    if(u->entry_pt == e) 
        u->entry_pt = e->onext; 
    if(v->entry_pt == e) 
        v->entry_pt = e->dnext; 
    if(Org(e->onext) == u) 
        e->onext->oprev = e->oprev; 
    else 
        e->onext->dprev = e->oprev; 
    if(Org(e->oprev) == u) 
        e->oprev->onext = e->onext; 
    else 
        e->oprev->dnext = e->onext; 
    if(Org(e->dnext) == v) 
        e->dnext->oprev = e->dprev; 
    else 
        e->dnext->dprev = e->dprev; 
    if(Org(e->dprev) == v) 
        e->dprev->onext = e->dnext; 
    else 
        e->dprev->dnext = e->dnext; 
    free_edge(e); 
} 

double px[MAXV], py[MAXV];

void read_points() 
{
	int i = 0;
    while (1)
    { 
		int t1, t2;
		scanf("%d", &t1);
		px[i] = t1;
		if (t1 == -1)
			break;
		scanf("%d", &t2);
		py[i] = t2;
		i++;
    }
	n = i;
} 

static void print_edges(int n) 
{ 
    edge *e_start, *e; 
    point *u, *v; 
    int i; 
    for(i = 0; i < n; i++) 
    { 
        u = &p_array[i]; 
        e_start = e = u->entry_pt; 
        do 
        { 
            v = Other_point(e, u); 
            if(u < v) 
            { 
                E[en].u = u - p_array, E[en].v = v - p_array; 
                E[en++].w = hypot(u->x - v->x, u->y - v->y); 
            } 
            e = Next(e, u); 
        }while(!Identical_refs(e, e_start)); 
    } 
} 

void merge_sort(point *p[], point *p_temp[], int l, int r) 
{ 
    int i, j, k, m; 
    if(r - l > 0) 
    { 
        m = (r + l) / 2; 
        merge_sort(p, p_temp, l, m); 
        merge_sort(p, p_temp, m + 1, r); 
        for(i = m + 1; i > l; i--) 
            p_temp[i - 1] = p[i - 1]; 
        for(j = m; j < r; j++) 
            p_temp[r + m - j] = p[j + 1]; 
        for(k = l; k <= r; k++) 
            if(p_temp[i]->x < p_temp[j]->x) 
            { 
                p[k] = p_temp[i]; 
                i = i + 1; 
            } 
            else if(p_temp[i]->x == p_temp[j]->x && p_temp[i]->y < p_temp[j]->y) 
            { 
                p[k] = p_temp[i]; 
                i = i + 1; 
            } 
            else 
            { 
                p[k] = p_temp[j]; 
                j = j - 1; 
            } 
    } 
} 

static void lower_tangent(edge *r_cw_l, point *s, edge *l_ccw_r, point *u, edge **l_lower, point **org_l_lower, edge **r_lower, point **org_r_lower) 
{ 
    edge *l, *r; 
    point *o_l, *o_r, *d_l, *d_r; 
    boolean finished; 
    l = r_cw_l; 
    r = l_ccw_r; 
    o_l = s; 
    d_l = Other_point(l, s); 
    o_r = u; 
    d_r = Other_point(r, u); 
    finished = FALSE; 
    while(!finished) 
        if(Cross_product_3p(o_l, d_l, o_r) > 0.0) 
        { 
            l = Prev(l, d_l); 
            o_l = d_l; 
            d_l = Other_point(l, o_l); 
        } 
        else if(Cross_product_3p(o_r, d_r, o_l) < 0.0) 
        { 
            r = Next(r, d_r); 
            o_r = d_r; 
            d_r = Other_point(r, o_r); 
        } 
        else 
            finished = TRUE; 
        *l_lower = l; 
        *r_lower = r; 
        *org_l_lower = o_l; 
        *org_r_lower = o_r; 
} 

static void merge(edge *r_cw_l, point *s, edge *l_ccw_r, point *u, edge **l_tangent) 
{ 
    edge *base, *l_cand, *r_cand; 
    point *org_base, *dest_base; 
    Real u_l_c_o_b, v_l_c_o_b, u_l_c_d_b, v_l_c_d_b; 
    Real u_r_c_o_b, v_r_c_o_b, u_r_c_d_b, v_r_c_d_b; 
    Real c_p_l_cand, c_p_r_cand; 
    Real d_p_l_cand, d_p_r_cand; 
    boolean above_l_cand, above_r_cand, above_next, above_prev; 
    point *dest_l_cand, *dest_r_cand; 
    Real cot_l_cand, cot_r_cand; 
    edge *l_lower, *r_lower; 
    point *org_r_lower, *org_l_lower; 
    lower_tangent(r_cw_l, s, l_ccw_r, u, &l_lower, &org_l_lower, &r_lower, &org_r_lower); 
    base = join(l_lower, org_l_lower, r_lower, org_r_lower, right); 
    org_base = org_l_lower; 
    dest_base = org_r_lower; 
    *l_tangent = base; 
    do 
    { 
        l_cand = Next(base, org_base); 
        r_cand = Prev(base, dest_base); 
        dest_l_cand = Other_point(l_cand, org_base); 
        dest_r_cand = Other_point(r_cand, dest_base); 
        Vector(dest_l_cand, org_base, u_l_c_o_b, v_l_c_o_b); 
        Vector(dest_l_cand, dest_base, u_l_c_d_b, v_l_c_d_b); 
        Vector(dest_r_cand, org_base, u_r_c_o_b, v_r_c_o_b); 
        Vector(dest_r_cand, dest_base, u_r_c_d_b, v_r_c_d_b); 
        c_p_l_cand = Cross_product_2v(u_l_c_o_b, v_l_c_o_b, u_l_c_d_b, v_l_c_d_b); 
        c_p_r_cand = Cross_product_2v(u_r_c_o_b, v_r_c_o_b, u_r_c_d_b, v_r_c_d_b); 
        above_l_cand = c_p_l_cand > 0.0; 
        above_r_cand = c_p_r_cand > 0.0; 
        if(!above_l_cand && !above_r_cand) 
            break; 
        if(above_l_cand) 
        { 
            Real u_n_o_b, v_n_o_b, u_n_d_b, v_n_d_b; 
            Real c_p_next, d_p_next, cot_next; 
            edge *next; 
            point *dest_next; 
            d_p_l_cand = Dot_product_2v(u_l_c_o_b, v_l_c_o_b, u_l_c_d_b, v_l_c_d_b); 
            cot_l_cand = d_p_l_cand / c_p_l_cand; 
            do 
            { 
                next = Next(l_cand, org_base); 
                dest_next = Other_point(next, org_base); 
                Vector(dest_next, org_base, u_n_o_b, v_n_o_b); 
                Vector(dest_next, dest_base, u_n_d_b, v_n_d_b); 
                c_p_next = Cross_product_2v(u_n_o_b, v_n_o_b, u_n_d_b, v_n_d_b); 
                above_next = c_p_next > 0.0; 
                if(!above_next)  
                    break; 
                d_p_next = Dot_product_2v(u_n_o_b, v_n_o_b, u_n_d_b, v_n_d_b); 
                cot_next = d_p_next / c_p_next; 
                if(cot_next > cot_l_cand) 
                    break; 
                delete_edge(l_cand); 
                l_cand = next; 
                cot_l_cand = cot_next; 
            }while(TRUE); 
        } 
        if(above_r_cand) 
        { 
            Real u_p_o_b, v_p_o_b, u_p_d_b, v_p_d_b; 
            Real c_p_prev, d_p_prev, cot_prev; 
            edge *prev; 
            point *dest_prev; 
            d_p_r_cand = Dot_product_2v(u_r_c_o_b, v_r_c_o_b, u_r_c_d_b, v_r_c_d_b); 
            cot_r_cand = d_p_r_cand / c_p_r_cand; 
            do 
            { 
                prev = Prev(r_cand, dest_base); 
                dest_prev = Other_point(prev, dest_base); 
                Vector(dest_prev, org_base, u_p_o_b, v_p_o_b); 
                Vector(dest_prev, dest_base, u_p_d_b, v_p_d_b); 
                c_p_prev = Cross_product_2v(u_p_o_b, v_p_o_b, u_p_d_b, v_p_d_b); 
                above_prev = c_p_prev > 0.0; 
                if(!above_prev) 
                    break; 
                d_p_prev = Dot_product_2v(u_p_o_b, v_p_o_b, u_p_d_b, v_p_d_b); 
                cot_prev = d_p_prev / c_p_prev; 
                if(cot_prev > cot_r_cand) 
                    break; 
                delete_edge(r_cand); 
                r_cand = prev; 
                cot_r_cand = cot_prev; 
            }while(TRUE); 
        } 
        dest_l_cand = Other_point(l_cand, org_base); 
        dest_r_cand = Other_point(r_cand, dest_base); 
        if(!above_l_cand || (above_l_cand && above_r_cand && cot_r_cand < cot_l_cand)) 
        { 
            base = join(base, org_base, r_cand, dest_r_cand, right); 
            dest_base = dest_r_cand; 
        } 
        else 
        { 
            base = join(l_cand, dest_l_cand, base, dest_base, right); 
            org_base = dest_l_cand; 
        } 
    }while(TRUE); 
} 

void divide(point *p_sorted[], int l, int r, edge **l_ccw, edge **r_cw) 
{ 
    int n; 
    int split; 
    edge *l_ccw_l, *r_cw_l, *l_ccw_r, *r_cw_r, *l_tangent; 
    edge *a, *b, *c; 
    Real c_p; 
    n = r - l + 1; 
    if(n == 2) 
    { 
        *l_ccw = *r_cw = make_edge(p_sorted[l], p_sorted[r]); 
    } 
    else if(n == 3) 
    { 
        a = make_edge(p_sorted[l], p_sorted[l + 1]); 
        b = make_edge(p_sorted[l + 1], p_sorted[r]); 
        splice(a, b, p_sorted[l + 1]); 
        c_p = Cross_product_3p(p_sorted[l], p_sorted[l + 1], p_sorted[r]); 
        if(c_p > 0.0) 
        { 
            c = join(a, p_sorted[l], b, p_sorted[r], right); 
            *l_ccw = a; 
            *r_cw = b; 
        } 
        else if(c_p < 0.0) 
        { 
            c = join(a, p_sorted[l], b, p_sorted[r], left); 
            *l_ccw = c; 
            *r_cw = c; 
        } 
        else 
        { 
            *l_ccw = a; 
            *r_cw = b; 
        } 
    } 
    else if(n > 3) 
    { 
        split = (l + r) / 2; 
        divide(p_sorted, l, split, &l_ccw_l, &r_cw_l); 
        divide(p_sorted, split+1, r, &l_ccw_r, &r_cw_r); 
        merge(r_cw_l, p_sorted[split], l_ccw_r, p_sorted[split + 1], &l_tangent); 
        if(Org(l_tangent) == p_sorted[l]) 
            l_ccw_l = l_tangent; 
        if(Dest(l_tangent) == p_sorted[r]) 
            r_cw_r = l_tangent; 
        *l_ccw = l_ccw_l; 
        *r_cw = r_cw_r; 
    } 
} 

int main(void) 
{ 
    int ca;
	for (scanf("%d", &ca); ca--;)
	{
		scanf("%d", &k);
        edge *l_cw, *r_ccw; 
        int i; 
        point **p_sorted, **p_temp; 
        read_points();
		if (k > n) k = n;
		if (n == 1)
		{
			printf("0\n");
			continue;
		}
		alloc_memory(n);
	    for (i = 0; i < n; i++)
		{
			p_array[i].x = px[i];
			p_array[i].y = py[i];
		}
        for(i = 0; i < n; i++) 
            p_array[i].entry_pt = NULL; 
        p_sorted = (point **)malloc((unsigned)n * sizeof(point *)); 
        p_temp = (point **)malloc((unsigned)n * sizeof(point *)); 
        for(i = 0; i < n; i++) 
            p_sorted[i] = p_array + i; 
        merge_sort(p_sorted, p_temp, 0, n - 1); 
        free((char *)p_temp); 
        divide(p_sorted, 0, n - 1, &l_cw, &r_ccw); 
        free((char *)p_sorted); 
        en = 0; 
        print_edges(n); 
        kruskal(); 
        free_memory(); 
    } 
    return 0; 
}