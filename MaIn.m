function MaIn()
s=serial('COM12');
fopen(s);
fid=fopen('telno.txt','r');
Tel_no=fscanf(fid,'%u\n');
fclose(fid);
[len, siz]=size(Tel_no);
fid2=fopen('data.txt','w');
if(fid2~=-1)
    ii=fscanf(fid2,'%u');
    fclose(fid2);
else
    ii=1;
end
    


for i=ii:len
    var=num2str(Tel_no(i));
    fid2=fopen('data.txt','w');
    fprintf(fid2,var);
    fclose(fid2);
       fid2=fopen('te1.txt','r');
       state=fscanf(fid2,'%u\n');
       fclose(fid2);
       
       if(state==1)
           
     arr=['ATD +91' var ';'];
     fprintf(s,arr);
  system('lv1.bat');
     fprintf(s,'ATH');
       else
           if(state==2)
           return;
           end
       end
       
       if(state==3)
              fid2=fopen('te1.txt','w');
              fprintf(fid2,'1');
              fclose(fid2);
              i=1;
       end
           
       
end
end



