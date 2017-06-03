/* Face recognition wrapper for the recognition method in the dlib examples.
 * Uses a neural network to extract a 128 dimensional embedding of a face. It
 * then uses a metric based connectivity criterion to use for the Chinese
 * Whispers clustering algorithm.
 *
 * Adapted from code in dlib examples. Thanks David King for the framework and
 * the pre-trained models, and of course the collective researchers who came up
 * with the architectures.
 * See: dnn_face_recognition_ex.cpp, dnn_mmod_face_detection_ex.cpp in dlib.
 *
 * Released into the public domain.
 * Explanation: http://creativecommons.org/licenses/publicdomain
 * If your legal jurisdiction does not recognise the public domain, then it is
 * licensed under Boost Software Licence.
 * Boost Licence: http://www.boost.org/users/license.html
 *
 * Model can be downloaded from:
 * http://dlib.net/files/dlib_face_recognition_resnet_model_v1.dat.bz2
 */


#ifndef _FACETOOLS_FACE_RECOGNISER_H_
#define _FACETOOLS_FACE_RECOGNISER_H_


// ## INCLUDES ################################################################

#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/dnn.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <string>

#include "face.h"


// ## NAMESPACES ##############################################################

namespace facetools {


// ## TYPE DEFINITIONS ########################################################

/* Templates that setup the neural network for generating the 128 dimensional embedding. */

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = dlib::add_prev1<block<N,BN,1,dlib::tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = dlib::add_prev2<dlib::avg_pool<2,2,2,2,dlib::skip1<dlib::tag2<block<N,BN,2,dlib::tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<dlib::con<N,3,3,1,1,dlib::relu<BN<dlib::con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET>
using ares = dlib::relu<residual<block,N,dlib::affine,SUBNET>>;

template <int N, typename SUBNET>
using ares_down = dlib::relu<residual_down<block,N,dlib::affine,SUBNET>>;

template <typename SUBNET>
using alevel0 = ares_down<256,SUBNET>;

template <typename SUBNET>
using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;

template <typename SUBNET>
using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;

template <typename SUBNET>
using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;

template <typename SUBNET>
using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;

using resnet_v1 = dlib::loss_metric<dlib::fc_no_bias<128,dlib::avg_pool_everything<alevel0<alevel1<alevel2<alevel3<alevel4<
  dlib::max_pool<3,3,2,2,dlib::relu<dlib::affine<dlib::con<32,7,7,2,2,dlib::input_rgb_image_sized<150>>>>>>>>>>>>>;


// ## CUSTOM STRUCTURES #######################################################

/**
 * Parameters for the FaceRecogniser class.
 */
struct face_recogniser_parameters_t
{
  /** Threshold used to determine when two faces are similar. */
  float face_difference_threshold;

  /** Whether to apply jitter transformations to images before calculating an embedding. */
  bool jitter_images;

  /** Path name for recogniser model. */
  std::string recogniser_model_file;

  face_recogniser_parameters_t()
  {
    recogniser_model_file = "dlib_face_recognition_resnet_model_v1.dat";
    jitter_images = false;
    face_difference_threshold = 0.4; /* Davis optimised for 0.6.  If you are looking at face clusters, then 0.4 might work better for minorities. */
  }
};


// ## TYPEDEFS, USING #########################################################

/* List of faces by label id (used in clustering algorithm). */
using facelist_t = std::vector<unsigned int>;


// ## CLASS DEFINITION ########################################################

class face_recogniser
{
public:
  /**
   * \param Parameters to use.
   */
  face_recogniser(const face_recogniser_parameters_t& params);


  /**
   * Get the embedding for the face specified.
   * \param input_face Face we want an embedding for.
   * \return 128 dimensional vector representing the face (embedding).
   */
  embedding_t get_embedding(const face& input_face);


  /**
   * Get the embedding for the list of faces specified.
   * \param input_faces List of faces we want an embedding for.
   * \return List of 128 dimensional vectors representing the face (embedding).
   */
  std::vector<embedding_t> get_embedding(const std::vector<face>& input_faces);


  /**
   * \return Whether the jitter_images parameter is set.
   */
  bool get_jitter() const noexcept;


  /**
   * Gets a list of distinct people found in your image. The indexes are the same as the embedding list.
   * \param embeddings List of face embeddings.
   * \return List of of embedding indices corresponding to faces.
   */
  std::vector<facelist_t> get_people(const std::vector<embedding_t>& embeddings);


  /**
   * Set the jitter_images parameter.
   * \param state New state to set.
   */
  void set_jitter(bool state) noexcept;


#ifndef _DEBUG_
private:
#endif

  /** See face_recogniser_parameters_t. */
  struct internal_parameters_t
  {
    float face_difference_threshold;
    bool jitter_images;
  } params_;

  resnet_v1 recogniser_; /* Recogniser neural network. */


  /**
   * Gets the faces not assigned by the Chinese whispers algorithm.
   * \param visited_face Status list for which face was visited (indexed by original face list).
   * \param labels_size Number of face cluster labels assigned by Chinese whispers.
   * \return Number of unassigned faces.
   */
  size_t chinese_whispers_get_unvisited_faces_(std::vector<bool>& visited_face, const size_t labels_size);


  /**
   * Add the unvisited faces to their own cluster.
   * \param people List of identified faces to modify.
   * \param visited_face Status list for which face was visited (indexed by original face list).
   * \param faces_size Number of faces in original list.
   * \param num_clusters Number of face clusters output by Chinese whispers.
   */
  void chinese_whispers_add_remaining_faces_(std::vector<facelist_t>& people, const std::vector<bool>& visited_face,
    const size_t faces_size, const size_t num_clusters);


  /**
   * Use Chinese whispers to sort faces into clusters (distinct people).
   * \param embeddings List of face embeddings to cluster.
   * \return List of identified faces.
   */
  std::vector<facelist_t> get_chinese_whispers_clusters_(const std::vector<embedding_t>& embeddings);


  /**
   * Apply random jitter transformations to the image.
   * \param image Image to apply jitters to.
   * \return List of jittered images.
   */
  std::vector<dlib::matrix<dlib::rgb_pixel>> jitter_image_(const dlib::matrix<dlib::rgb_pixel>& image);

};


} // NAMESPACE facetools

#endif //_FACETOOLS_FACE_RECOGNISER_H_