function getdat2()
tic
o1=audiorecorder(44100, 16,1);
recordblocking(o1,1);
y=getaudiodata(o1);
Y=fft(y);
max=0.0;
m_i=0;
for i=20:1000
    if(abs(Y(i,1))>max)
        max=abs(Y(i,1));
        m_i=i;
    end
end
while(~((m_i>780) && (m_i<820)))
 
recordblocking(o1,1);
y=getaudiodata(o1);
Y=fft(y);
max=0.0;
m_i=0;
for i=20:1000
    if(abs(Y(i,1))>max)
        max=abs(Y(i,1));
        m_i=i;
    end
end
end
%% %%%%%%%%%%%
system('lv2.bat');
end
