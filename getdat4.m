function getdat4()

fis=fopen('data.txt','r');
name1=fscanf(fis,'%s');
name2=strcat(name1,'.txt');
%[y Fs nbit]=wavread(name1);
%Y1=1:2048;
%Y2=1:2048;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% added part
   %%Open the analog device and channels
%AO = analogoutput('winsound',0);
%chan = addchannel(AO,1);

% 44,100 Hz, 1 seconds of data
%duration = 1;
%SampleRate = 44100;
%set(AO,'SampleRate',SampleRate)
%set(AO,'TriggerType','Manual')
%NumSamples = SampleRate*duration;
% Create a signal that we would like to send, 500 Hz sin wave
%x = linspace(0,2*pi*500,NumSamples);
%  data = sin(x)';
% Put the data in the buffer, start the device, and trigger
%  putdata(AO,data)
%  start(AO)
%  trigger(AO)
% clean up, close down
%  wait(AO,5)
%  delete(AO)
%  clear AO



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Fs = 44100;
%y = wavrecord(5*Fs,Fs,'double');
%AI = analoginput('winsound',0);
%chan = addchannel(AI,1);duration = 10;
%SampleRate = 44100;
%set(AI,'SampleRate',SampleRate);
%set(AI,'SamplesPerTrigger',duration*SampleRate);
%set(AI,'TriggerType','Manual');start(AI);trigger(AI);data = getdata(AI);
%WAIT(AI,5)
%delete(AI)
%clear AI

o1=audiorecorder(Fs,16,1);
recordblocking(o1,10);
y=getaudiodata(o1);




%% end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Y=fft(y);
%set the filter here
%2048-1 to Fs   1-1024==1-Fs/2Hz
%f1  f2--
%the band widthis 100 Hz
%centers=d at 600 1200
[s0 s1]=size(Y);
r1=450/Fs*s0;
r2=555/Fs*s0;

Y1=Y;
for ite=2:(s0/2)+1
    if((ite<r1)||(ite>r2))
        Y1(ite,1)=0;
        Y1(s0-ite+2,1)=0;
    end
   
end
Y2=Y;
y1=ifft(Y1);

%%%
r1=950/Fs*s0;
r2=1050/Fs*s0;

for ite=2:s0/2+1
    if((ite<r1)||(ite>r2))
        Y2(ite,1)=0;
        Y2(s0-ite+2,1)=0;
    end
end
y2=ifft(Y2,s0);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[a1 a2]=size(y);
L=1000;
b1=a1/Fs;
b2=Fs/100;
siz=1+(round(a1*100/Fs));
siz1=1+(round(siz/16));
vals0=1:siz;
vals1=1:siz;
sens=1:siz1;
pres=1:siz1;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
j_ind=1;
index0=1;
% logic  :
glo_ave=0.0;
for s_v0=1:a1
    glo_ave=glo_ave+abs(y1(s_v0));

end

glo_ave=glo_ave/a1;

for s_v=1:(a1-1)
   t_t=y1(s_v)-y1(s_v+1);
   t_t0=abs(t_t)/glo_ave;
   if(t_t0>=30)
       index0=s_v;
       s_v=a1;
       break;
   end
end
    

%end

for i=index0:b2:a1
    maax=0.0;
    if((i+b2)>a1)
           for j=i:(a1)
               maax=maax+abs(y1(j,1));
           end
          tot=maax/(a1-i);
            if(tot>(glo_ave*1.4))      %%%%%%%%%%%%%%%%%%%%%5  change  _f
                vals0(j_ind)=1;
            else
                
                vals0(j_ind)=0;
            end
           
            
    else
        for j=i:(i+b2-1)
              maax=maax+abs(y1(j,1));
        end
           tot=maax/b2;
             if(tot>(glo_ave*1.4))      %%%%%%%%%%%%%%%%%%%%%5  change  _f
                vals0(j_ind)=1;
            else
                
                vals0(j_ind)=0;
            end
           
            
           
           
        
    end
    j_ind=j_ind+1;
end


%%%%%%%%%%%%%%%%other sensor

j_ind=1;



%logic

index0=1;
% logic  :
glo_ave=0.0;
for s_v0=1:a1
    glo_ave=glo_ave+abs(y2(s_v0));

end

glo_ave=glo_ave/a1;

for s_v=1:(a1-1)
   t_t=y2(s_v)-y2(s_v+1);
   t_t0=abs(t_t)/glo_ave;
   if(t_t0>=30)
       index0=s_v;
       s_v=a1;
       break;
   end
end

%
for i=index0:b2:a1
    maax=0.0;
    if((i+b2)>a1)
           for j=i:(a1)
               maax=maax+abs(y2(j,1));
           end
          tot=maax/(a1-i);
             if(tot>(glo_ave*1.4))      %%%%%%%%%%%%%%%%%%%%%5  change  _f
                vals0(j_ind)=1;
            else
                
                vals0(j_ind)=0;
            end
           
            
    else
        for j=i:(i+b2-1)
              maax=maax+abs(y2(j,1));
        end
           tot=maax/b2;
              if(tot>(glo_ave*1.4))      %%%%%%%%%%%%%%%%%%%%%5  change  _f
                vals0(j_ind)=1;
              else
                
                vals0(j_ind)=0;
              end
           
           
            %i=i+b2;
           
        
    end
    j_ind=j_ind+1;
end


%%%%%%%%%%%%  entering value %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55555
 j_ind=1;
for i=1:16:siz
    if((i+15)<siz)
        
        for jz=0:15
            sens(j_ind)=vals0(i+jz)*(2^jz);
            pres(j_ind)=vals1(i+jz)*(2^jz);
        end
    else
        
        for jz=0:(siz-i-1)
            sens(j_ind)=vals0(i+jz)*(2^jz);
            pres(j_ind)=vals1(i+jz)*(2^jz);
        end
    end
    
    %i=i+16;
    j_ind=j_ind+1;
    
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fid=fopen(name2,'w+');
fprintf(fid,'%s - %d %d %d %d %d %2.0f\n HAHA \n',date,clock);
for k=1:siz1
    fprintf(fid,'%d %d\n',sens(k),pres(k));
end   
fclose(fid);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

end