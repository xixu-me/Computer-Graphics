// ZBuffer.cpp: implementation of the CZBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "ZBuffer.h"
#include "math.h"
#define Round(d) int(floor(d+0.5))//四舍五入宏定义
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZBuffer::CZBuffer()
{
	P=NULL;
	pHeadE=NULL;
	pCurrentB=NULL;
	pEdge=NULL;
	pCurrentE=NULL;
	pHeadB=NULL;
	zBuffer=NULL;
}

CZBuffer::~CZBuffer()
{
	if(P!=NULL)
	{
		delete []P;
		P=NULL;
	}
	for(int i=0;i<Width;i++)
	{
		delete[] zBuffer[i];
		zBuffer[i]=NULL;
	}
	if(zBuffer!=NULL)
	{
		delete zBuffer;
		zBuffer=NULL;
	}	
	ClearMemory();
}

void CZBuffer::SetPoint(CPi3 p[],int m)
{
	if(P!=NULL)
	{
		delete []P;
		P=NULL;
	}
	P=new CPi3[m];
    for(int i=0;i<m;i++)
		P[i]=p[i];	

	//add
	  
	//     P[0].c=CRGB(1.0,0.0,0.0);
		 // P[1].c=CRGB(0.0,1.0,0.0);
	//	   P[2].c=CRGB(0.0,0.0,1.0);
	 
	PNum=m;
}

void CZBuffer::CreateBucket()//创建桶表
{
	int yMin,yMax;
	yMin=yMax=P[0].y;
	for(int i=1;i<PNum;i++)//查找多边形所覆盖的最小和最大扫描线
	{
		if(P[i].y<yMin)
			yMin=P[i].y;//扫描线的最小值
		if(P[i].y>yMax)
			yMax=P[i].y;//扫描线的最大值
	}
	for(int y=yMin;y<=yMax;y++)
	{
		if(yMin==y)//建立桶头结点
		{
			pHeadB=new CBucket;//建立桶的头结点
			pCurrentB=pHeadB;//pCurrentB为CBucket当前结点指针
			pCurrentB->ScanLine=yMin;
			pCurrentB->pET=NULL;//没有链接边表
			pCurrentB->pNext=NULL;
		}
		else//其他扫描线
		{
			pCurrentB->pNext=new CBucket;//建立桶的其他结点
			pCurrentB=pCurrentB->pNext;
			pCurrentB->ScanLine=y;
			pCurrentB->pET=NULL;
			pCurrentB->pNext=NULL;
		}
	}
}

void CZBuffer::CreateEdge()//创建边表
{
	for(int i=0;i<PNum;i++)
	{
		pCurrentB=pHeadB;
		int j=(i+1)%PNum;//边的第二个顶点，P[i]和P[j]构成边
		if(P[i].y<P[j].y)//边的终点比起点高
		{
			pEdge=new CAET;
			pEdge->x=P[i].x;//计算ET表的值
			pEdge->yMax=P[j].y;
			pEdge->k=(P[j].x-P[i].x)/(P[j].y-P[i].y);//代表1/k
			pEdge->ps=P[i];//绑定顶点和颜色
			pEdge->pe=P[j];
			pEdge->pNext=NULL;
			while(pCurrentB->ScanLine!=P[i].y)//在桶内寻找该边的yMin
			{
				pCurrentB=pCurrentB->pNext;//移到yMin所在的桶结点
			}		
		}
		if(P[j].y<P[i].y)//边的终点比起点低
		{
			pEdge=new CAET;
			pEdge->x=P[j].x;
			pEdge->yMax=P[i].y;
			pEdge->k=(P[i].x-P[j].x)/(P[i].y-P[j].y);
			pEdge->ps=P[i];
			pEdge->pe=P[j];
			pEdge->pNext=NULL;
			while(pCurrentB->ScanLine!=P[j].y)
			{
				pCurrentB=pCurrentB->pNext;
			}
		}
		if(int(P[j].y)!=P[i].y)
		{
			pCurrentE=pCurrentB->pET;
			if(pCurrentE==NULL)
			{
				pCurrentE=pEdge;
				pCurrentB->pET=pCurrentE;
			}
			else
			{
				while(pCurrentE->pNext!=NULL)
				{
					pCurrentE=pCurrentE->pNext;
				}
				pCurrentE->pNext=pEdge;
			}
		}
	}
}

void CZBuffer::Gouraud(CDC *pDC)//填充多边形
{
	double	CurDeep=0.0;//当前扫描线的深度
	double	DeepStep=0.0;//当前扫描线随着x增长的深度步长
	double	A,B,C,D;//平面方程Ax+By+Cz＋D=0的系数
	CVector V01(P[0],P[1]),V02(P[0],P[2]);
	CVector VN=Cross(V01,V02);
	A=VN.x;B=VN.y;C=VN.z;
	D=-A*P[0].x-B*P[0].y-C*P[0].z;
	DeepStep=-A/C;//计算扫描线深度步长增量
	CAET *pT1,*pT2;
	pHeadE=NULL;	
	for(pCurrentB=pHeadB;pCurrentB!=NULL;pCurrentB=pCurrentB->pNext)
	{
		for(pCurrentE=pCurrentB->pET;pCurrentE!=NULL;pCurrentE=pCurrentE->pNext)
		{
			pEdge=new CAET;
			pEdge->x=pCurrentE->x;
			pEdge->yMax=pCurrentE->yMax;
			pEdge->k=pCurrentE->k;
			pEdge->ps=pCurrentE->ps;
			pEdge->pe=pCurrentE->pe;
			pEdge->pNext=NULL;
			AddEt(pEdge);
		}
		ETOrder();	
		pT1=pHeadE;
		if(pT1==NULL)
			return;
		while(pCurrentB->ScanLine>=pT1->yMax)//下闭上开
		{
			CAET * pAETTEmp=pT1;			
			pT1=pT1->pNext;
			delete pAETTEmp;
			pHeadE=pT1;
			if(pHeadE==NULL)
				return;
		}
		if(pT1->pNext!=NULL)
		{
			pT2=pT1;
			pT1=pT2->pNext;
		}
		while(pT1!=NULL)
		{
			if(pCurrentB->ScanLine>=pT1->yMax)//下闭上开
			{
				CAET* pAETTemp =pT1;
				pT2->pNext=pT1->pNext;				
				pT1=pT2->pNext;
				delete pAETTemp;
			}
			else
			{
				pT2=pT1;
				pT1=pT2->pNext;
			}
		}
		CRGB ca,cb,cf;//ca、cb代边上任意点的颜色，cf代表面上任意点的颜色
		ca=Interpolation(pCurrentB->ScanLine,pHeadE->ps.y,pHeadE->pe.y,pHeadE->ps.c,pHeadE->pe.c);
		cb=Interpolation(pCurrentB->ScanLine,pHeadE->pNext->ps.y,pHeadE->pNext->pe.y,pHeadE->pNext->ps.c,pHeadE->pNext->pe.c);
		BOOL bInFlag=FALSE;//区间内外测试标志，初始值为假表示区间外部
		double xleft,xright;//扫描线和有效边相交区间的起点和终点坐标
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->pNext)
		{
			if(FALSE==bInFlag)
			{
				xleft=pT1->x;
				CurDeep=-(xleft*A+pCurrentB->ScanLine*B+D)/C;//z=-(Ax+By-D)/C
				bInFlag=TRUE;
			}
			else
			{
				xright=pT1->x;
				for(double x=xleft;x<xright;x++)//左闭右开
				{
					cf=Interpolation(x,xleft,xright,ca,cb);
					if(CurDeep<=zBuffer[Round(x)+Width/2][pCurrentB->ScanLine+Height/2])//如果当前采样点的深度小于帧缓冲器中原采样点的深度
					{
						zBuffer[Round(x)+Width/2][pCurrentB->ScanLine+Height/2]=CurDeep;//使用当前采样点的深度更新深度缓冲器
						pDC->SetPixelV(Round(x),pCurrentB->ScanLine,RGB(cf.red*255,cf.green*255,cf.blue*255));//绘制当前采样点
					}
					CurDeep+=DeepStep;
				}
				bInFlag=FALSE;
			}
		}
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->pNext)//边的连续性
			pT1->x=pT1->x+pT1->k;
	}
}

void CZBuffer::AddEt(CAET *pNewEdge)//合并ET表
{
	CAET *pCE;
	pCE=pHeadE;
	if(pCE==NULL)
	{
		pHeadE=pNewEdge;
		pCE=pHeadE;
	}
	else
	{
		while(pCE->pNext!=NULL)
		{
			pCE=pCE->pNext;
		}
		pCE->pNext=pNewEdge;
	}
}

void CZBuffer::ETOrder()//边表的冒泡排序算法
{
	CAET *pT1,*pT2;
	int Count=1;
	pT1=pHeadE;
	if(pT1==NULL)
		return;
	if(pT1->pNext==NULL)//如果该ET表没有再连ET表
		return;//桶结点只有一条边，不需要排序
	while(pT1->pNext!=NULL)//统计边结点的个数
	{
		Count++;
		pT1=pT1->pNext;
	}
	for(int i=0;i<Count-1;i++)//冒泡排序
	{
		CAET **pPre=&pHeadE;
		pT1=pHeadE;
		for(int j=0;j<Count-1-i;j++)
		{
			pT2=pT1->pNext;
		
			if ((pT1->x>pT2->x)||((pT1->x==pT2->x)&&(pT1->k>pT2->k)))
			{
				pT1->pNext=pT2->pNext;
				pT2->pNext=pT1;
				*pPre=pT2;
				pPre=&(pT2->pNext);//调整位置为下次遍历准备
			}
			else
			{
				pPre=&(pT1->pNext);
				pT1=pT1->pNext;
			}
		}
	}
}

CRGB CZBuffer::Interpolation(double t,double t1,double t2,CRGB clr1,CRGB clr2)//颜色线性插值
{
	CRGB color;
	color=(t-t2)/(t1-t2)*clr1+(t-t1)/(t2-t1)*clr2;
	return color;
}

void CZBuffer::InitDeepBuffer(int Width,int Height,double Depth)//初始化深度缓冲
{
	int i;
	this->Width=Width,this->Height=Height;
	zBuffer=new double *[Width];
	for( i=0;i<Width;i++)
		zBuffer[i]=new double[Height];
	for(i=0;i<Width;i++)//初始化深度缓冲
		for(int j=0;j<Height;j++)
			zBuffer[i][j]=Depth;
}

void CZBuffer::ClearMemory()
{
	DeleteAETChain(pHeadE);
	CBucket *pBucket=pHeadB;
	while (pBucket !=NULL)//针对每一个桶
	{
		CBucket *pBucketTemp=pBucket->pNext;
		DeleteAETChain(pBucket->pET);
		delete pBucket;
		pBucket=pBucketTemp;
	}
	pHeadB=NULL;
	pHeadE=NULL;
}

void CZBuffer::DeleteAETChain(CAET* pAET)
{
	while (pAET!=NULL)
	{
		CAET *pAETTemp=pAET->pNext;
		delete pAET;
		pAET=pAETTemp;
	}
}