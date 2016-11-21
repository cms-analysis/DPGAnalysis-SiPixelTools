#cmsRun pxl.py maxEvents=-1 useTemplateReco=1 outputFileName=PixelTripletHistos_Tem_2016_v2.root            GlobalTag=80X_dataRun2_Prompt_v15
#cmsRun pxl.py maxEvents=-1 useTemplateReco=1 outputFileName=PixelTripletHistos_Tem_2016_v3.root            useLocalDBTemplate=1
#cmsRun pxl.py maxEvents=-1 useTemplateReco=1 outputFileName=PixelTripletHistos_Tem_2016_v4.root            useLocalDBTemplate=1
#cmsRun pxl.py maxEvents=-1 useTemplateReco=0 outputFileName=PixelTripletHistos_Gen_2016_v2.root            GlobalTag=80X_dataRun2_Prompt_v15
#cmsRun pxl.py maxEvents=-1 useTemplateReco=0 outputFileName=PixelTripletHistos_Gen_2016_v3.root            useLocalDBGenErr=1 useLocalDBLA=1
#cmsRun pxl.py maxEvents=-1 useTemplateReco=0 outputFileName=PixelTripletHistos_Gen_2016_v4.root            useLocalDBGenErr=1 useLocalDBLA=1
#cmsRun pxl.py maxEvents=-1 useTemplateReco=0 outputFileName=PixelTripletHistos_Gen_2016_v3_GenErrOnly.root useLocalDBGenErr=1
#cmsRun pxl.py maxEvents=-1 useTemplateReco=0 outputFileName=PixelTripletHistos_Gen_2016_v4_GenErrOnly.root useLocalDBGenErr=1
#cmsRun pxl.py maxEvents=-1 useTemplateReco=0 outputFileName=PixelTripletHistos_Gen_2016_v1_LAOnly.root     useLocalDBLA=1
#cmsRun pxl.py maxEvents=-1 useTemplateReco=0 outputFileName=PixelTripletHistos_Gen_2016_v3_LAOnly.root     useLocalDBLA=1
#cmsRun pxl.py maxEvents=-1 useTemplateReco=0 outputFileName=PixelTripletHistos_Gen_2016_v4_LAOnly.root     useLocalDBLA=1
root 'print_residuals.C("PixelTripletHistos_Tem_2016_v2.root","282735")' | grep -v Processing
root 'print_residuals.C("PixelTripletHistos_Tem_2016_v3.root","282735")' | grep -v Processing | grep "201"
root 'print_residuals.C("PixelTripletHistos_Tem_2016_v4.root","282735")' | grep -v Processing | grep "201"
root 'print_residuals.C("PixelTripletHistos_Gen_2016_v2.root","282735")' | grep -v Processing | grep "201"
root 'print_residuals.C("PixelTripletHistos_Gen_2016_v3.root","282735")' | grep -v Processing | grep "201"
root 'print_residuals.C("PixelTripletHistos_Gen_2016_v4.root","282735")' | grep -v Processing | grep "201"
root 'print_residuals.C("PixelTripletHistos_Gen_2016_v1_LAOnly.root","282735")' | grep -v Processing | grep "201"
root 'print_residuals.C("PixelTripletHistos_Gen_2016_v3_LAOnly.root","282735")' | grep -v Processing | grep "201"
root 'print_residuals.C("PixelTripletHistos_Gen_2016_v4_LAOnly.root","282735")' | grep -v Processing | grep "201"
root 'print_residuals.C("PixelTripletHistos_Gen_2016_v3_GenErrOnly.root","282735")' | grep -v Processing | grep "201"
root 'print_residuals.C("PixelTripletHistos_Gen_2016_v4_GenErrOnly.root","282735")' | grep -v Processing | grep "201"
