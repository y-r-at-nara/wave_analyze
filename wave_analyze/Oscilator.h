#ifndef YR_WAVEANALYZE_OSCILATOR_H_
#define YR_WAVEANALYZE_OSCILATOR_H_

#include <cinttypes>

namespace yr {
namespace waveanalyze {

class Oscilator {
public:
	/**
	 * �R���X�g���N�^�B��������ݒ肵���p�����[�^�͕ύX�s�B
	 */
	Oscilator(
		const uint32_t sampleing_rate,///< [in] �T���v�����O���g��
		const double wb,			///< [in] ���U���g��
		const double zeta			///< [in] �����萔
		);

	/**
	 * �f�X�g���N�^�B�قډ������Ȃ��B
	 */
	~Oscilator();

	/**
	 * �O�͂���͂���
	 */
	void InputExternalForce(
		const double force			///< [in] �O��
		);

	/**
	 * ���݈ʒu���擾����
	 * 
	 * @retval ���݈ʒu
	 */
	double GetPos();

	/**
	 * ���ݐU�����擾����
	 *
	 * @retval ���ݐU��
	 */
	double GetAmplitude();

	/**
	 * ���ݎ������擾����
	 *
	 * @retval ���ݎ���
	 */
	uint64_t GetTime();

	/**
	 * ���܂܂ł̌v�Z���ʂ��N���A����
	 */
	void Clear();

private:
	//�T���v�����O���g��
	const uint32_t sampling_rate_;
	//���g��
	const double   wn_;
	//�����萔
	const double   zeta_;

	//�ʒu
	double pos_;
	double pos_diff_;
	//���x
	double vel_;
	double vel_diff_;

	//�U�� (���ݐU��)
	double amplitude_;
	//�ʒu�����������ǂ���
	bool is_pos_increasing_;

	//���ݎ���
	uint64_t time_;
};

}; //namespace waveanalyze
}; //namespace yr

#endif //YR_WAVE_OSCILATOR_H_
