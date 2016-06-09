void polin2(x1a,x2a,ya,m,n,x1,x2,y,dy)
float x1a[],x2a[],**ya,x1,x2,*y,*dy;
int m,n;
{
	int k,j;
	float *ymtmp,*yntmp,*vector();
	void polint(),free_vector();

	ymtmp=vector(1,m);
	yntmp=vector(1,n);
	for (j=1;j<=m;j++) {
		for (k=1;k<=n;k++)
			yntmp[k]=ya[j][k];
		polint(x2a,yntmp,n,x2,&ymtmp[j],dy);
	}
	polint(x1a,ymtmp,m,x1,y,dy);
	free_vector(yntmp,1,n);
	free_vector(ymtmp,1,m);
}

