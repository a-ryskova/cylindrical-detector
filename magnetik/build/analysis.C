void analysis() {
    // Установка кодировки UTF-8 для ROOT
    gStyle->SetTitleFont(43, "xyz");  // Font type
    gStyle->SetLabelFont(43, "xyz");
    gStyle->SetTitleSize(20, "xyz");
    gStyle->SetLabelSize(20, "xyz");
    
    // Открытие файла с данными
    TFile *f = new TFile("detector_hits.root");
    if (!f->IsOpen()) {
        printf("Error: Cannot open detector_hits.root\n");
        return;
    }
    
    TTree *t = (TTree*)f->Get("Hits");
    if (!t) {
        printf("Error: Cannot find Hits tree\n");
        return;
    }

    // Создание канваса с английскими подписями
    TCanvas *c1 = new TCanvas("c1", "Detector Analysis", 1200, 800);
    c1->Divide(2,2);

    // Гистограмма энергий
    c1->cd(1);
    t->Draw("Energy>>hE(100,0,100)", "", "");
    TH1F *hE = (TH1F*)gDirectory->Get("hE");
    hE->SetTitle("Energy Distribution;Energy [MeV];Events");
    hE->SetLineColor(kBlue);
    gPad->SetLogy();

    // Гистограмма позиций X
    c1->cd(2);
    t->Draw("PositionX>>hX(100,-100,100)", "", "");
    TH1F *hX = (TH1F*)gDirectory->Get("hX");
    hX->SetTitle("X Position Distribution;X [cm];Events");
    hX->SetLineColor(kRed);

    // Гистограмма позиций Y
    c1->cd(3);
    t->Draw("PositionY>>hY(100,-100,100)", "", "");
    TH1F *hY = (TH1F*)gDirectory->Get("hY");
    hY->SetTitle("Y Position Distribution;Y [cm];Events");
    hY->SetLineColor(kGreen+2);

    // Гистограмма позиций Z
    c1->cd(4);
    t->Draw("PositionZ>>hZ(100,-150,150)", "", "");
    TH1F *hZ = (TH1F*)gDirectory->Get("hZ");
    hZ->SetTitle("Z Position Distribution;Z [cm];Events");
    hZ->SetLineColor(kMagenta);

    // Сохранение результатов
    c1->SaveAs("analysis_results.pdf");
    c1->SaveAs("analysis_results.root");
}
