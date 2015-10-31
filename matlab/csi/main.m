%
%simon <nicksimon@gmail.com>
%2015-05-19 15:42
%
global m;
m=2; 
  
% read files
csi_trace_alice=read_bf_file('data/alice.dat')
csi_trace_bob=read_bf_file('data/bob.dat')
%csi_trace_alice=read_bf_file('sample/tmp_927_16/alice.dat')
%csi_trace_bob=read_bf_file('sample/tmp_927_16/bob.dat')

%get same seq
%cal();
for i=1:size(csi_trace_alice,1)
    csi_entry_alice = csi_trace_alice{i};
    seq_alice(i) = csi_entry_alice.seq;
end
for i=1:size(csi_trace_bob,1)
    csi_entry_bob = csi_trace_bob{i};
    seq_bob(i) = csi_entry_bob.seq;
end
j=1;
for i=1:max(max(seq_alice(:)),max(seq_bob(:)))
    if (find(seq_alice(:)==i) & find(seq_bob(:)==i))
        seq_both(j)=i;
        j=j+1;
    end
end

%filter csi
%alice
j=1;
for i=1:size(csi_trace_alice,1)
    csi_entry_alice = csi_trace_alice{i};
    seq = csi_entry_alice.seq;
    if(find(seq_both==seq))
        alice(j)=csi_entry_alice;
        j=j+1;
    end
end
%bob
j=1;
for i=1:size(csi_trace_bob,1)
    csi_entry_bob = csi_trace_bob{i};
    seq = csi_entry_bob.seq;
    if(find(seq_both==seq))
        bob(j)=csi_entry_bob;
        j=j+1;
    end
end
clear i j seq* csi_entry*;
gap=100;
key=[0;0];
a=round(rand(1,1)*10+10);
for index=1:30
    ret=[0;0];
    csi_alice=extract_m_csi(alice(1:5),index);
    csi_bob=extract_m_csi(bob(1:5),index);
    num=floor(size(csi_alice,2)/gap);
    for i=1:num
        sindex=(i-1)*gap+1;
        eindex=sindex+29;
        generate_key(csi_alice(sindex:eindex),csi_bob(sindex:eindex));
        if(ans~=-1)
            ret=[ret ans];
        end
    end
    sindex=num*gap+1;
    eindex=size(csi_alice,2);
    generate_key(csi_alice(sindex:eindex),csi_bob(sindex:eindex));
    if(ans~=-1)
        ret=[ret ans];
    end
    key=[key ret];
    key;
end
clear i j m num sindex eindex gap ans ret index a