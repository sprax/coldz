void splie2(x1a,x2a,ya,m,n,y2a)
float x1a[],x2a[],**ya,**y2a;
int m,n;
{
	int k,j;
	float *ytmp,*y2tmp,*vector();
	void spline(),free_vector();

	ytmp=vector(1,n);
	y2tmp=vector(1,n);
	for (j=1;j<=m;j++) {
		for (k=1;k<=n;k++)
			ytmp[k]=ya[j][k];
		spline(x2a,ytmp,n,1.0e30,1.0e30,y2tmp);
		for (k=1;k<=n;k++)
			y2a[j][k]=y2tmp[k];
	}
	free_vector(y2tmp,1,n);
	free_vector(ytmp,1,n);
}

