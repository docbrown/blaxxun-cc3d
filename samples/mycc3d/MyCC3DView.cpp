// MyCC3DView.cpp : implementation of the CMyCC3DView class
//

#include "stdafx.h"
#include "MyCC3D.h"

#include "MyCC3DDoc.h"
#include "MyCC3DView.h"

#include "DOpenUrl.h"

#include "CC3DApi.h"
#include "CC3DHelpers.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCC3DView

IMPLEMENT_DYNCREATE(CMyCC3DView, CView)

BEGIN_MESSAGE_MAP(CMyCC3DView, CView)
	//{{AFX_MSG_MAP(CMyCC3DView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_OPEN_URL, OnFileOpenUrl)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_URL, OnUpdateFileOpenUrl)
	ON_COMMAND(ID_ADD_GEOMETRY, OnAddGeometry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCC3DView construction/destruction

CMyCC3DView::CMyCC3DView() : m_cc3d(NULL)
{
	// TODO: add construction code here

}

CMyCC3DView::~CMyCC3DView()
{
}

BOOL CMyCC3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyCC3DView drawing

void CMyCC3DView::OnDraw(CDC* pDC)
{
	CMyCC3DDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMyCC3DView diagnostics

#ifdef _DEBUG
void CMyCC3DView::AssertValid() const
{
	CView::AssertValid();
}

void CMyCC3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyCC3DDoc* CMyCC3DView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyCC3DDoc)));
	return (CMyCC3DDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyCC3DView message handlers

void CMyCC3DView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    
	if (m_cc3d) // need to resize CC3D
    {
        m_cc3d->MoveWindow(0,0,cx,cy);
    }

	
}

// called on first display of a new document
// initialize CC3D if not done yet, and load the file
// for an OCX control use OnOpen
void CMyCC3DView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	CMyCC3DDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	// TODO: Add your specialized code here and/or call the base class
	
	if (m_cc3d == NULL) { // startup of cc3d 
		ASSERT(m_cc3d == NULL);

		m_cc3d = new CblaxxunCC3D();

		CRect m_rect;
		GetClientRect(&m_rect);
		if (m_cc3d->Create(NULL, WS_VISIBLE, m_rect, this, 1 ))
		{

		}
		else
		{
			AfxMessageBox("Error loading blaxxun CC3D\nPlease check your installation");
			delete m_cc3d;
			m_cc3d = NULL;
		
		}
	}

	// get the file pathname from document
	if (m_cc3d && pDoc) {
		CString path = pDoc->GetPathName();

		if (path.GetLength()>0) { // non empty document
			m_cc3d->SetUrl(path);	// and open it 
		}		
	}

	
}

int CMyCC3DView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	
	return 0;
}


void CMyCC3DView::OpenFile(LPCTSTR pathName)
{

	if (m_cc3d) {
		m_cc3d->SetUrl(pathName);
	}


}

// save the scene graph stored in CC3D to a file
// directory must exist

BOOL CMyCC3DView::SaveFile(LPCTSTR pathName)
{
	if (m_cc3d) {
		return m_cc3d->saveWorld(pathName);
	}
	return FALSE;
}

void CMyCC3DView::OnFinalRelease() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::OnFinalRelease();
}


// destroy CC3D, for an OCX control use OnHide()
void CMyCC3DView::OnDestroy() 
{

	// release any node handles here
	//
	if (m_cc3d) { // destroy CC3D
        m_cc3d->DestroyWindow();
        delete m_cc3d;
        m_cc3d = NULL;

	}

	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CMyCC3DView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (m_cc3d)  // no need to erase the background, CC3D covers the window
		return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

void CMyCC3DView::OnFileOpenUrl() 
{
	// TODO: Add your command handler code here

	
	DOpenUrl d;
	d.m_url = "http://www.blaxxun.com/vrml/home/home.wrl";

	if (d.DoModal() == IDOK) {
		OpenFile(d.m_url);
	}
	

	
}

void CMyCC3DView::OnUpdateFileOpenUrl(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMyCC3DView::OnAddGeometry() 
{
	// TODO: Add your command handler code here

	// test code

	TRACE("Creating node : \n");

	CNode transform(m_cc3d->createNode("Transform"));
	CNode sphere(m_cc3d->createNode("Sphere"));
	CNode shape(m_cc3d->createNode("Shape"));
	CNode appearance(m_cc3d->createNode("Appearance"));
	CNode material(m_cc3d->createNode("Material"));

	static float somewhere = -2.0;

	if (transform) { // method to check if node / event has succesfully been created / retreived
		TRACE("Setting event In \n");

		CEventInSFVec3f trans(transform.getEventIn("set_translation"));

		float v[3];

		if (trans) {
			trans.setValue(somewhere,1,0); // send eventIn to transfrom.set_translation
			somewhere+=1.0f,

			
			TRACE("Setting event In \n");
			/* build the following scene graph by hand  :: 
			    Trasnform { translation someWhere 1 0 
				children 
					Shape { appearance Appearance { material Material {diffuseColor 0.9,0.5,0.5 } }
						geometry Sphere {}
					}

				}
			*/

			CEventInSFColor(material.getEventIn("diffuseColor")).setValue(0.9f,0.5f,0.5f);

			CEventInSFNode(appearance.getEventIn("material")).setValue(material);
			CEventInSFNode(shape.getEventIn("geometry")).setValue(sphere);
			CEventInSFNode(shape.getEventIn("appearance")).setValue(appearance);

			CEventInMFNode(transform.getEventIn("addChildren")).set1Value(0,shape);

			// getting in eventOut for testing 
			CEventOutSFVec3f(transform.getEventOut("translation")).getValue(v);

			TRACE("Transform.translation == %g %g %g \n",v[0],v[1],v[2]);

			// add node to scene graph 
			m_cc3d->addNode(transform);

			if (true) {
				CMyEventOutObserver *observer = new CMyEventOutObserver();
				
				CNode toucher(m_cc3d->createNode("TouchSensor"));

				CEventInMFNode(transform.getEventIn("addChildren")).set1Value(0,toucher);
				
				// getting in eventOut for testing 
				CEventOut(toucher.getEventOut("isActive")).advise(observer,NULL);
				CEventOut(toucher.getEventOut("isOver")).advise(observer,NULL);
				CEventOut(toucher.getEventOut("touchTime")).advise(observer,NULL);

				// observer released if CC3D deletes the node and the advise connection
				// could also keep reference to observer AddRef // Release and do an undavise


					
			}

		}	
	}

	
}
