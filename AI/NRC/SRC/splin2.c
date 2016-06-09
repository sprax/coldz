void splin2(x1a,x2a,ya,y2a,m,n,x1,x2,y)
float x1a[],x2a[],**ya,**y2a,x1,x2,*y;
int m,n;
{
	int k,j;
	float *ytmp,*y2tmp,*yytmp,*vector();
	void spline(),splint(),free_vector();

	ytmp=vector(1,n);
	y2tmp=vector(1,n);
	yytmp=vector(1,n);
	for (j=1;j<=m;j++) {
		for (k=1;k<=n;k++) {
			ytmp[k]=ya[j][k];
			y2tmp[k]=y2a[j][k];
		}
		splint(x2a,ytmp,y2tmp,n,x2,&yytmp[j]);
	}
	spline(x1a,yytmp,m,1.0e30,1.0e30,y2tmp);
	splint(x1a,yytmp,y2tmp,m,x1,y);
	free_vector(yytmp,1,n);
	free_vector(y2tmp,1,n);
	free_vector(ytmp,1,n);
}

