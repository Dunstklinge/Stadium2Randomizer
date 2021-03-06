// CDistDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CDistDialog.h"
#include "afxdialogex.h"
#include "resource.h"

#include <vector>

#include "GlobalRandom.h"

// CDistDialog dialog

IMPLEMENT_DYNAMIC(CDistDialog, CDialogEx)

CDistDialog::CDistDialog(CWnd* pParent, DiscreteDistribution dist)
	: CDialogEx(IDD_DISTRDISTDIALOG, pParent), dist(dist), useMinMax(false)
{
	ignoreUpdates = false;
	for (auto it = VarCtrlBegin(); it != VarCtrlEnd(); ++it) {
		it->owner = this;
	}
}
CDistDialog::CDistDialog(CWnd* pParent, DiscreteDistribution dist, int minMin, int maxMax)
	: CDialogEx(IDD_DISTRDISTDIALOG, pParent), dist(dist), useMinMax(true), minMin(minMin), maxMax(maxMax)
{
	ignoreUpdates = false;
	for (auto it = VarCtrlBegin(); it != VarCtrlEnd(); ++it) {
		it->owner = this;
	}
}

CDistDialog::~CDistDialog()
{
	::KillTimer(this->GetSafeHwnd(), redrawTimer);
}

void CDistDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CANVAS, stCanvas);
	DDX_Control(pDX, IDC_CBBTYPE, cbbType);
	DDX_Control(pDX, IDC_EDMAX, varMax.ed);
	DDX_Control(pDX, IDC_EDMIN, varMin.ed);
	DDX_Control(pDX, IDC_SLIDERMAX, varMax.slider);
	DDX_Control(pDX, IDC_SLIDERMIN, varMin.slider);
	DDX_Control(pDX, IDC_SPINMAX, varMax.spin);
	DDX_Control(pDX, IDC_SPINMIN, varMin.spin);

	for (int i = 0; i < _countof(varCtrls); i++) {
		DDX_Control(pDX, IDC_STATIC_1 + i, varCtrls[i].st);
		DDX_Control(pDX, IDC_ED_1 + i, varCtrls[i].ed);
		DDX_Control(pDX, IDC_SPIN_1 + i, varCtrls[i].spin);
		DDX_Control(pDX, IDC_SLIDER_1 + i, varCtrls[i].slider);
	}
	
}


BEGIN_MESSAGE_MAP(CDistDialog, CDialogEx)
	ON_WM_DRAWITEM()
	ON_CBN_SELCHANGE(IDC_CBBTYPE, &CDistDialog::OnCbnSelchangeCbbtype)
	//	ON_EN_UPDATE(IDC_EDMIN, &CDistDialog::OnEditUpdated)
	//	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDERMIN, &CDistDialog::OnSliderUpdated)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDMIN, IDC_ED_5, &CDistDialog::OnEditUpdated)
	ON_NOTIFY_RANGE(TRBN_THUMBPOSCHANGING, IDC_SLIDERMIN, IDC_SLIDER_5, &CDistDialog::OnSliderUpdated)
	ON_NOTIFY_RANGE(UDN_DELTAPOS, IDC_SPINMIN, IDC_SPIN_5, &CDistDialog::OnSpinUpdated)
//	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDistDialog message handlers


BOOL CDistDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	cbbType.AddString("Uniform");
	cbbType.AddString("Normal");
	cbbType.AddString("Triangle");
	switch (dist.GetType()) {
	case DiscreteDistribution::UNIFORM:
		cbbType.SetCurSel(0);
		break;
	case DiscreteDistribution::NORMAL:
		cbbType.SetCurSel(1);
		break;
	case DiscreteDistribution::TRIANGLE:
		cbbType.SetCurSel(2);
		break;
	}

	varMin.useDouble = false;
	varMin.ival = &dist.Min();
	varMax.useDouble = false;
	varMax.ival = &dist.Max();
	varMin.Pair();
	varMax.Pair();
	for (auto& var : varCtrls) 
		var.Pair();

	if (!useMinMax) {
		maxMax = 100;
		minMin = 1;
		dist.SetMinMax(1, 100);
		varMin.SetMinMax(1, 100);
		varMax.SetMinMax(1, 100);
		varMin.SetVal(1);
		varMax.SetVal(100);
		varMin.EnableWindow(FALSE);
		varMax.EnableWindow(FALSE);
	}
	else {
		varMin.SetMinMax(minMin, maxMax);
		varMax.SetMinMax(minMin, maxMax);
		varMin.SetVal(dist.Min());
		varMax.SetVal(dist.Max());
	}

	OnCbnSelchangeCbbtype();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDistDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDCtl == IDC_CANVAS) {
		constexpr COLORREF colorBkg = RGB(255,255,255);
		constexpr COLORREF colorGraph = RGB(100, 150, 255);

		CPen penBkg(PS_SOLID, 1, colorBkg);
		CPen penGraph(PS_SOLID, 1, colorGraph);

		CDC dc; dc.Attach(lpDrawItemStruct->hDC);
		CRect canvas;
		stCanvas.GetClientRect(&canvas);

		int distWidth = dist.Max() - dist.Min() + 1;
		int canvasWidth = canvas.Width();
		double stepPerPixel = (double)distWidth / canvasWidth;

		const int sampleN = 5000;
		int sampleMax = 0;
		std::vector<int> samples;
		samples.resize(distWidth);
		for (int i = 0; i < sampleN; i++) {
			int num = dist(Random::Generator);
			samples[num - dist.Min()]++;
			sampleMax = max(sampleMax, samples[num - dist.Min()]);
		}

		double currN = 0;
		for (int i = 0; i < canvasWidth; i++) {
			int concreteStep = currN;
			if (concreteStep >= distWidth) concreteStep = distWidth - 1;
			double percent = (double)samples[concreteStep] / sampleMax * 0.66;
			currN += stepPerPixel;

			int pixelHeight = canvas.Height() * percent;

			dc.SelectObject(&penGraph);
			dc.MoveTo(canvas.left + i, canvas.bottom);
			dc.LineTo(canvas.left + i, canvas.bottom - pixelHeight);
			dc.SelectObject(&penBkg);
			dc.LineTo(canvas.left + i, canvas.bottom - canvas.Height());
		}

		dc.Detach();
	}
	else {
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}


void CDistDialog::OnCbnSelchangeCbbtype()
{
	int selected = cbbType.GetCurSel();
	switch (selected) {
	case DiscreteDistribution::UNIFORM:
		SwitchToUniform();
		break;
	case DiscreteDistribution::NORMAL:
		SwitchToNormal();
		break;
	case DiscreteDistribution::TRIANGLE:
		SwitchToTriangle();
		break;
	default:
		return;
	}
	stCanvas.RedrawWindow();
}

void CDistDialog::SwitchToUniform() {
	if(dist.GetType() != DiscreteDistribution::UNIFORM)
		dist.MakeUniform();
	for (int i = 0; i < _countof(varCtrls); i++) {
		varCtrls[i].EnableWindow(FALSE);
	}
}
void CDistDialog::SwitchToNormal() {
	if (dist.GetType() != DiscreteDistribution::NORMAL)
		dist.MakeNormal({ 1, 0 });

	varCtrls[0].EnableWindow(TRUE);
	varCtrls[0].Reset(&dist.np.nStandardDerivations, 0, 20);
	varCtrls[1].EnableWindow(TRUE);
	int maxTrans = (maxMax - minMin) * 0.75;
	varCtrls[1].Reset(&dist.np.xTranslation, -maxTrans, maxTrans);
	for (int i = 2; i < _countof(varCtrls); i++) {
		varCtrls[i].EnableWindow(FALSE);
	}
}
void CDistDialog::SwitchToTriangle() {
	if (dist.GetType() != DiscreteDistribution::TRIANGLE)
		dist.MakeTriangle({ 0, 0 });

	int maxTrans = (maxMax - minMin) * 0.75;
	varCtrls[0].EnableWindow(TRUE);
	varCtrls[0].Reset(&dist.tp.xPeak, -maxTrans, maxTrans);
	varCtrls[1].EnableWindow(TRUE);
	varCtrls[1].Reset(&dist.tp.xTranslation, -maxTrans, maxTrans);
	
	for (int i = 2; i < _countof(varCtrls); i++) {
		varCtrls[i].EnableWindow(FALSE);
	}
}

void CDistDialog::OnEditUpdated(UINT nId)
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	if (!ignoreUpdates) {
		for (auto it = VarCtrlBegin(); it != VarCtrlEnd(); ++it) {
			if (it->ed.GetDlgCtrlID() == nId) {
				it->SyncFrom(true, false, false);
			}
		}
		if (!timerStarted) {
			timerStarted = true;
			redrawTimer = ::SetTimer(this->GetSafeHwnd(), 0, REDRAW_TIMER, 0);
		}
	}
}


void CDistDialog::OnSliderUpdated(UINT nId, NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	if (!ignoreUpdates) {
		for (auto it = VarCtrlBegin(); it != VarCtrlEnd(); ++it) {
			if (it->slider.GetDlgCtrlID() == nId) {
				it->SyncFrom(false, false, true);
			}
		}
		if (!timerStarted) {
			timerStarted = true;
			redrawTimer = ::SetTimer(this->GetSafeHwnd(), 0, REDRAW_TIMER, 0);
		}
	}
}

void CDistDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (!ignoreUpdates) {
		for (auto it = VarCtrlBegin(); it != VarCtrlEnd(); ++it) {
			if (pScrollBar && pScrollBar->GetSafeHwnd() == it->slider.GetSafeHwnd()) {
				it->SyncFrom(false, false, true);
			}
		}
		if (!timerStarted) {
			timerStarted = true;
			redrawTimer = ::SetTimer(this->GetSafeHwnd(), 0, REDRAW_TIMER, 0);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDistDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (!ignoreUpdates) {
		for (auto it = VarCtrlBegin(); it != VarCtrlEnd(); ++it) {
			if (pScrollBar && pScrollBar->GetSafeHwnd() == it->spin.GetSafeHwnd()) {
				it->SyncFrom(false, true, false);
			}
		}
		if (!timerStarted) {
			timerStarted = true;
			redrawTimer = ::SetTimer(this->GetSafeHwnd(), 0, REDRAW_TIMER, 0);
		}
	}
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CDistDialog::OnSpinUpdated(UINT nId, NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	/*if (!ignoreUpdates) for (auto it = VarCtrlBegin(); it != VarCtrlEnd(); ++it) {
		if (it->spin.GetDlgCtrlID() == nId) {
			it->SyncFrom(false, true, false);
		}
	}*/
}

void CDistDialog::VarCtrl::Pair()
{
	spin.SetBuddy(&ed);
}

void CDistDialog::VarCtrl::Reset(int* ival, int min, int max) 
{
	useDouble = false;
	this->ival = ival;
	SetMinMax(min, max);
	SetVal(*ival);
}
void CDistDialog::VarCtrl::Reset(double* dval, double min, double max) 
{
	useDouble = true;
	this->dval = dval;
	SetMinMax(min, max);
	SetVal(*dval);
}

void CDistDialog::VarCtrl::SetMinMax(int min, int max)
{
	useDouble = false;
	imin = min;
	imax = max;

	spin.SetRange(imin, imax);
	
	slider.SetRange(imin, imax);

	int old = *ival;
	std::clamp(*ival, imin, imax);
	if (*ival != old) SetVal(*ival);
}

void CDistDialog::VarCtrl::SetMinMax(double min, double max)
{
	useDouble = true;
	dmin = min;
	dmax = max;

	spin.SetRange32(0, DOUBLE_STEPS);
	slider.SetRange(0, DOUBLE_STEPS);

	double old = *dval;
	std::clamp(*dval, dmin, dmax);
	if (*dval != old) SetVal(*dval);
}

void CDistDialog::VarCtrl::SetVal(int v, bool refreshEdit, bool refreshSpinner, bool refreshSlider)
{
	*ival = v;
	std::clamp(*ival, imin, imax);
	if (this == &owner->varMin) {
		int max = *owner->varMax.ival;
		if (max < *ival) {
			owner->varMax.SetVal(*ival);
		}
	}
	else if (this == &owner->varMax) {
		int min = *owner->varMin.ival;
		if (min > *ival) {
			owner->varMin.SetVal(*ival);
		}
	}

	owner->ignoreUpdates = true;
	if (refreshEdit) {
		CString str;
		str.Format(TEXT("%d"), *ival);
		ed.SetWindowText(str);
	}
	if (refreshSpinner) {
		spin.SetPos32(*ival);
	}
	if (refreshSlider) {
		slider.SetPos(*ival);
	}
	owner->ignoreUpdates = false;
}

void CDistDialog::VarCtrl::SetVal(double v, bool refreshEdit, bool refreshSpinner, bool refreshSlider)
{
	*dval = v;
	std::clamp(*dval, dmin, dmax);
	
	owner->ignoreUpdates = true;
	if (refreshEdit) {
		CString str;
		str.Format(TEXT("%f"), *dval);
		ed.SetWindowText(str);
	}
	int virtPos = (*dval - dmin) / (dmax - dmin) * DOUBLE_STEPS;
	if (refreshSpinner) {
		spin.SetPos32(virtPos);
	}
	if (refreshSlider) {
		slider.SetPos(virtPos);
	}
	owner->ignoreUpdates = false;
}

void CDistDialog::VarCtrl::SyncFrom(bool edit, bool spinner, bool slider)
{
	if (edit) {
		CString text;
		ed.GetWindowText(text);
		if (useDouble) {
			SetVal(_tstof(text), false, true, true);
		}
		else {
			SetVal(_tstoi(text), false, true, true);
		}
	}
	else if (spinner) {
		int pos = spin.GetPos32();
		if (useDouble) {
			double d = pos / (double)DOUBLE_STEPS * (dmax - dmin) + dmin;
			SetVal(d, true, false, true);
		}
		else {
			SetVal(pos, true, false, true);
		}
	}
	else if (slider) {
		int pos = this->slider.GetPos();
		if (useDouble) {
			double d = pos / (double)DOUBLE_STEPS * (dmax - dmin) + dmin;
			SetVal(d, true, true, false);
		}
		else {
			SetVal(pos, true, true, false);
		}
	}
}



void CDistDialog::OnTimer(UINT_PTR nIDEvent)
{
	this->stCanvas.RedrawWindow();
	CDialogEx::OnTimer(nIDEvent);
	::KillTimer(this->GetSafeHwnd(), 0);
	timerStarted = false;
}
