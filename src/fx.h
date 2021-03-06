#ifndef METAMORPH_FX_H
#define METAMORPH_FX_H

#include "notesegmentation/segmentation.h"
#include "simpl/simpl.h"
#include "exceptions.h"
#include "spec_env.h"
#include "transformations.h"

#ifndef TWELFTH_ROOT_2
#define TWELFTH_ROOT_2 1.0594630943592953
#endif

using namespace std;


namespace metamorph
{


typedef double sample;

class FX {
    protected:
        int _frame_size;
        int _hop_size;
        int _max_partials;
        int _current_segment;
        int _previous_segment;

        std::vector<HarmonicTransformation*> _harm_trans;
        std::vector<SpecEnvTransformation*> _specenv_trans;
        std::vector<ResidualTransformation*> _residual_trans;
        std::vector<TransientTransformation*> _transient_trans;

        sample _harmonic_scale;
        sample _residual_scale;
        sample _transient_scale;

        bool _preserve_transients;
        bool _transient_substitution;
        int _new_transient_size;
        int _transient_sample;
        sample* _new_transient;

        std::vector<sample> _input;

        int _fade_duration;
        sample* _fade_in;
        sample* _fade_out;

        notesegmentation::RTSegmentation _ns;

        simpl::Frame* _frame;
        simpl::Frame* _residual_frame;
        simpl::Frame* _prev_frame;
        simpl::PeakDetection* _pd;
        simpl::PartialTracking* _pt;
        simpl::Synthesis* _synth;
        simpl::Residual* _residual;

        void setup_frame(int input_size, int output_size);
        void cleanup_frame();

        bool _create_env;
        bool _apply_env;
        sample _env_interp;
        int _env_size;
        int _env_order;
        int _env_stable_partial_duration;
        sample _bin_size;
        std::vector<sample> _env;
        std::vector<sample> _new_env;
        std::vector<sample> _env_freqs;
        std::vector<sample> _env_mags;
        std::vector<long> _partial_lifetimes;
        simpl::Frame* _env_frame;
        simpl::PartialTracking* _env_pt;
        SpectralEnvelope* _spec_env;

        void reset_fade_windows();
        void reset_envelope_data();

        void create_envelope(simpl::Frame* frame);
        void apply_envelope(simpl::Frame* frame);

    public:
        FX();
        ~FX();

        void reset();

        int frame_size();
        virtual void frame_size(int new_frame_size);
        int hop_size();
        virtual void hop_size(int new_hop_size);
        int max_partials();
        virtual void max_partials(int new_max_partials);

        void add_transformation(HarmonicTransformation* trans);
        void add_transformation(SpecEnvTransformation* trans);
        void add_transformation(ResidualTransformation* trans);
        void add_transformation(TransientTransformation* trans);

        void clear_harmonic_transformations();
        void clear_specenv_transformations();
        void clear_residual_transformations();
        void clear_transient_transformations();
        void clear_transformations();

        // -------------------------------------------------------------------
        // Harmonic transformations
        sample harmonic_scale();
        virtual void harmonic_scale(sample new_harmonic_scale);

        bool preserve_envelope();
        void preserve_envelope(bool preserve);
        sample env_interp();
        void env_interp(sample new_env_interp);
        int env_size();
        void apply_envelope(int env_size, sample* env);
        void apply_envelope(std::vector<sample>& env);
        bool apply_envelope();
        void apply_envelope(bool new_apply_env);
        void clear_envelope();

        // -------------------------------------------------------------------
        // Residual transformations
        sample residual_scale();
        virtual void residual_scale(sample new_residual_scale);

        // -------------------------------------------------------------------
        // Transient transformations
        sample transient_scale();
        virtual void transient_scale(sample new_transient_scale);

        bool preserve_transients();
        void preserve_transients(bool preserve);

        bool transient_substitution();
        void transient_substitution(bool substitute);
        void new_transient(int new_transient_size, sample* new_transient);
        void clear_new_transient();

        // -------------------------------------------------------------------
        // Processing
        virtual void process_frame(int input_size, sample* input,
                                   int output_size, sample* output);
        virtual void process(long input_size, sample* input,
                             long output_size, sample* output);
};


} // end of namespace metamorph

#endif
