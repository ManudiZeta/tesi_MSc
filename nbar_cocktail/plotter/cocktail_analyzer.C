#include "TFile.h"
#include <Riostream.h>
#include "TCanvas.h"
#include "TH1D.h"
#include "TTree.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLine.h"
#include "TString.h"

void imgen(bool choice)
{
    
    TString name_file = (choice == 0) ? "../../../collection__output.root" : "../../../nog_out_05012026_2.root";
    
    if(name_file == "../../../collection__output.root")
    {
        cout<< "File: "<<name_file<<endl;
        gStyle->SetOptStat(1);
        ifstream in_file(name_file);
        
        if(!in_file)
        {
            cout<<"The root file doesn't exist. Generate it. \n";
            return;
        }
        in_file.close();
        TFile myf(name_file);
        
        TTree *tree = (TTree*)myf.Get("tree");
        TCanvas *tela = new TCanvas();
        
        tree->Draw("vpho_r_mRecoil>>h(100,0,2)","vpho_r_mRecoil>0 && vpho_r_mRecoil<2 && nROE_Charged__bo__bc == 0");
        TH1* h = (TH1*)gDirectory->Get("h");
        h->SetXTitle("m_{recoil} [GeV]");
        h->SetYTitle("counts []");
        h->SetTitle("Recoil mass of p+, #pi-, #gamma");
        tela->SaveAs("images/ranked_gamma/pdf/mRecoil_1.pdf");
        tela->SaveAs("images/ranked_gamma/root/mRecoil_1.root");
        
        myf.Close();
        delete tela;
    }
    
    
    
    
    else if(name_file == "../../../nog_out_05012026_2.root")
    {
        cout<< "File: "<<name_file<<endl;
        gStyle->SetOptStat(1);
        ifstream in_file(name_file);
        
        if(!in_file)
        {
            cout<<"The root file doesn't exist. Generate it. \n";
            return;
        }
        in_file.close();
        TFile myf(name_file);
        
        TTree *tree = (TTree*)myf.Get("tree");
        TCanvas *tela = new TCanvas();
        
        tree->Draw("vpho_r_mRecoil>>h(100,0,2)","vpho_r_mRecoil>0 && vpho_r_mRecoil<2 && nROE_Charged__bo__bc == 0 && alpha<0.35 && nbar_mcPDG == -2112");
        TH1* h = (TH1*)gDirectory->Get("h");
        h->SetXTitle("m_{recoil} [GeV]");
        h->SetYTitle("counts []");
        h->SetTitle("Recoil mass of p+, #pi-");
        tela->SaveAs("images/ranked_nogamma/pdf/mRecoil_3.pdf");
        tela->SaveAs("images/ranked_nogamma/root/mRecoil_3.root");
        
        myf.Close();
        delete tela;
        
    }
}



void multi_grand(TString var, TString cut2, TString cut3)
{
    gStyle->SetOptStat(0);
    string in_1;
    TFile *myf_1 = nullptr;
    
    
    in_1 = "../../../tot_nog_06012026.root";
    
    ifstream in_f1(in_1);
    
    if(!in_f1)
    {
        cout<<"Selected root file doesn't exist. Please, generate it. \n";
        return;
    }
    in_f1.close();
    
    myf_1 = new TFile("../../../tot_nog_06012026.root");
    
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    TTree *tree_2 = (TTree*)myf_1->Get("tree");
    TTree *tree_3 = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    TString var_1 =  var;
    TString var_2 =  var;
    TString var_3 =  var;
    cout<<"var_1 = "<<var_1<<endl;
    cout<<"var_2 = "<<var_2<<endl;
    cout<<"var_3 = "<<var_1<<endl;
    
    TString drawExpr1 = var_1 + ">>histo1(256,0,2)";
    TString drawExpr2 = var_2 + ">>histo2(256,0,2)";
    TString drawExpr3 = var_3 + ">>histo3(256,0,2)";
    
    TString cut_2 = cut2;
    TString cut_3 = cut3;
    
    cout<<"cut_2 = "<<cut_2<<endl;
    cout<<"cut_3 = "<<cut_3<<endl;

    
    tree_1->Draw(drawExpr1, "", "goff");
    tree_2->Draw(drawExpr2, cut_2, "goff");
    tree_3->Draw(drawExpr3, cut_3, "goff");

        
    delete c1;
    TH1 *histo1 = (TH1D*)gDirectory->Get("histo1");
    TH1 *histo2 = (TH1D*)gDirectory->Get("histo2");
    TH1 *histo3 = (TH1D*)gDirectory->Get("histo3");

    
    histo1->SetLineColor(kBlue);
    histo2->SetLineColor(kRed);
    histo3->SetLineColor(kGreen);
    
    histo1->SetFillColor(kBlue);
    histo2->SetFillColor(kRed);
    histo3->SetFillColor(kGreen);
    
    TString title_x = "recoil mass [#frac{GeV}{c^{2}}]";
    //TString title_x = "clusterSecondMoment []";
    histo1->GetXaxis()->SetTitle(title_x);
    histo1->GetYaxis()->SetTitle("counts");
    
    
    TString title = "";
    histo1->SetTitle(title);
    
    
    cout<<"NEntries_1 = "<<histo1->GetEntries()<<endl;
    cout<<"NEntries_2 = "<<histo2->GetEntries()<<endl;
    cout<<"NEntries_3 = "<<histo3->GetEntries()<<endl;
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(histo1,"No additional cuts","l");
    leg->AddEntry(histo2,"a","l");
    leg->AddEntry(histo3,"b","l");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    histo1->DrawCopy("HIST");
    histo2->DrawCopy("HIST SAMES");
    histo3->DrawCopy("HIST SAMES");
    leg->Draw("SAME");
    
    TString title_out = "../images/continuum/mRecoil_comparison.pdf";
    tela->SaveAs(title_out);
    
}


void multi_grand_real(TString var, TString cut1)
{
    gStyle->SetOptStat(0);
    string in_1, in_2;
    TFile *myf_1 = nullptr;
    
    
    in_1 = "../../../RealSel_nog_cocktail.root";
    
    ifstream in_f1(in_1);
    
    if(!in_f1)
    {
        cout<<"Selected root file doesn't exist. Please, generate it. \n";
        return;
    }
    in_f1.close();
    
    myf_1 = new TFile("../../../RealSel_nog_cocktail.root");
    
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    TString var_1 =  var;

    cout<<"var_1 = "<<var_1<<endl;
    
    TString drawExpr1 = var_1 + ">>histo1(256,0,2)";

    
    TString cut_1 = cut1;
    cout<<"cut_1 = "<<cut_1<<endl;

    
    tree_1->Draw(drawExpr1, cut_1, "goff");


    
    delete c1;
    TH1 *histo1 = (TH1D*)gDirectory->Get("histo1");
    TH1 *histofill = new TH1D();

    
    histo1->SetLineColor(kBlue);
    
    TString title_x = "recoil mass [#frac{GeV}{c^{2}}]";
    //TString title_x = "clusterSecondMoment []";
    histo1->GetXaxis()->SetTitle(title_x);
    histo1->GetYaxis()->SetTitle("counts");
    
    TString title = "";
    histo1->SetTitle(title);
    
    double max = histo1->GetBinContent(histo1->GetMaximumBin());
    
    cout<<"NEntries_1 = "<<histo1->GetEntries()<<endl;
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    
    
    
    TLine *tl_1 = new TLine(0.8,0,0.8,max);
    tl_1->SetLineColor(kBlack);
    
    TLine *tl_2 = new TLine(1.1,0,1.1,max);
    tl_2->SetLineColor(kBlack);
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    histo1->DrawCopy("HIST");
    tl_1->Draw("SAME");
    tl_2->Draw("SAME");
    
    TString title_out = "../images/continuum/mRecoil_comparison_real_new.pdf";
    tela->SaveAs(title_out);
    
}
