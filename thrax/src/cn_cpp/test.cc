#include "fst/script/print-impl.h"
#include "fst/fst.h"
#include "fst/fstlib.h"
#include "fst/vector-fst.h"




fst::Fst<fst::StdArc> * LoadFst(const std::string& fst)
{

  try {
		std::istringstream is(fst, std::ios_base::in | std::ios_base::binary);
		std::string name="phone_syms";
		fst::FstReadOptions opts(name);
		fst::Fst<fst::StdArc> * new_decode_fst = fst::Fst<fst::StdArc>::Read(is, opts);
		if (!new_decode_fst) {
			throw std::runtime_error("FST decoding graph not read.");
		}
		return new_decode_fst;
	} catch (std::runtime_error& e) {

	}
  return NULL;
}


template <class Arc, class I>
inline void MakeByteAcceptor(std::string &input,
                          fst::MutableFst<Arc>* ofst) {
    typedef typename Arc::StateId StateId;
    typedef typename Arc::Weight Weight;

    ofst->DeleteStates();
    StateId cur_state = ofst->AddState();
    ofst->SetStart(cur_state);
    for (int i=0; i<input.size(); i++){
        int idx = static_cast<int>(input[i]);
        StateId next_state = ofst->AddState();
        Arc arc(idx, idx, Weight::One(), next_state);
        ofst->AddArc(cur_state, arc);
        cur_state = next_state;
    }
    ofst->SetFinal(cur_state, Weight::One());
}




int main(int args, char *argv[]){
    // because thrax fst use one-Byte-char as label

    std::string input_str = "10月4日";
    std::string fst_path = "";

    auto fst = LoadFst(fst_path);

    fst::StringPrinter<fst::StdArc> printer(fst::StringTokenType::BYTE);
    fst::VectorFst<fst::StdArc> result;
    fst::VectorFst<fst::StdArc> input, output, result;
    
    MakeByteAcceptor(input_str, &input);

    fst::Compose(input, *fst, &result);

    fst::Project(&result, fst::PROJECT_OUTPUT);

    fst::ShortestPath(result, &output);

    // lattice to string
    std::string tmp_str;
    std::string output_str;
    printer(output, &tmp_str);
    for (unsigned char c: tmp_str) {
        if (c != 0) output_str += c;
    }

    std::cout<<output_str<<"\n";

    return 0;
}