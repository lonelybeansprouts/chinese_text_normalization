#include "fst/script/print-impl.h"
#include "fst/fst.h"
#include "fst/fstlib.h"
#include "fst/vector-fst.h"






template <class Arc>
inline void MakeByteAcceptor(std::string &input,
                          fst::MutableFst<Arc>* ofst) {
    typedef typename Arc::StateId StateId;
    typedef typename Arc::Weight Weight;

    ofst->DeleteStates();
    StateId cur_state = ofst->AddState();
    ofst->SetStart(cur_state);
    for (int i=0; i<input.size(); i++){
        unsigned char label = input.at(i);
        StateId next_state = ofst->AddState();
        Arc arc(label, label, Weight::One(), next_state);
        ofst->AddArc(cur_state, arc);
        cur_state = next_state;
    }
    ofst->SetFinal(cur_state, Weight::One());
}


template <class Arc>
inline void MakeByteAcceptorXX(std::string &input,
                          fst::MutableFst<Arc>* ofst) {
    typedef typename Arc::StateId StateId;
    typedef typename Arc::Weight Weight;
    
    unsigned char d = '#';

    ofst->DeleteStates();
    StateId cur_state = ofst->AddState();
    ofst->SetStart(cur_state);
    for (int i=0; i<input.size(); i++){
        unsigned char label = input.at(i);
        if (label == d){
            StateId next_state = ofst->AddState();
            ofst->AddArc(cur_state, Arc(0, 0, Weight::One(), next_state));
            ofst->AddArc(cur_state, Arc((unsigned char)('|'), (unsigned char)('|'), -0.00001, next_state));
            cur_state = next_state;
        }else{
          unsigned char label = input.at(i);
          StateId next_state = ofst->AddState();
          Arc arc(label, label, Weight::One(), next_state);
          ofst->AddArc(cur_state, arc);
          cur_state = next_state;
	}
    }
    ofst->SetFinal(cur_state, Weight::One());
}





int main(int args, char *argv[]){
    // because thrax fst use one-Byte-char as label

    std::string input_str = "十#乘#十#一#等#于#一#百#一#十#一";
    std::string fst_path = "../../cn/ITN.fst";


    fst::Fst<fst::StdArc> * fst = fst::Fst<fst::StdArc>::Read(fst_path);

    fst::StringPrinter<fst::StdArc> printer(fst::StringTokenType::BYTE);
    fst::VectorFst<fst::StdArc> input, output, result;
    
    MakeByteAcceptorXX<fst::StdArc>(input_str, &input);

    fst::Compose(input, *fst, &result);

    fst::ShortestPath(result, &output);
   
    fst::VectorFst<fst::StdArc> * output_cpy = new fst::VectorFst<fst::StdArc>(output);

    { 
        fst::Project(&output, fst::PROJECT_OUTPUT);
        // lattice to string
        std::string tmp_str;
        std::string output_str;
        printer(output, &tmp_str);
        for (unsigned char c: tmp_str) {
            if (c != 0) output_str += c;
        }

        std::cout<<output_str<<"\n";
    }

    { 
        fst::Project(output_cpy, fst::PROJECT_INPUT);
        // lattice to string
        std::string tmp_str;
        std::string output_str;
        printer(*output_cpy, &tmp_str);
        for (unsigned char c: tmp_str) {
            if (c != 0) output_str += c;
        }

        std::cout<<output_str<<"\n";
    }
    return 0;
}
