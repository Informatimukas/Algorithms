ld Len(ld ax, ld ay) { return sqrt(ax * ax + ay * ay); }

void Intersect(ll x1, ll y1, ll r1, ll x2, ll y2, ll r2)
{
	ld d = Len(x1 - x2, y1 - y2);
	if (r1 + r2 + eps < d) return;
	if (min(r1, r2) + d + eps < max(r1, r2)) return;
	if (fabs(d) < eps) return;
	ld a = (r1 * r1 - r2 * r2 + d * d) / (2.0l * d);
	ld h = sqrt(max(0.0l, r1 * r1 - a * a));
	ld px = x1 + (x2 - x1) / d * a;
	ld py = y1 + (y2 - y1) / d * a;
	ldld res1 = ldld(px + (y2 - y1) / d * h, py + (x1 - x2) / d * h);
	ldld res2 = ldld(px + (y1 - y2) / d * h, py + (x2 - x1) / d * h);
	seq.push_back(res1);
	seq.push_back(res2);
}
