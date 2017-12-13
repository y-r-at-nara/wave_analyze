#include "Oscilator.h"

#include <cstdio>
#include <algorithm>
#include <cinttypes>

using namespace yr;
using namespace yr::waveanalyze;

//=============================================================================
//=============================================================================
// PUBLIC METHODS
//=============================================================================
//=============================================================================

//=============================================================================
// Oscilator::Constructor
//=============================================================================

/**
 * �R���X�g���N�^�B��������ݒ肵���p�����[�^�͕ύX�s�B
 */
Oscilator::Oscilator(
	const uint32_t sampleing_rate,///< [in] �T���v�����O���g��
	const double   wn,			  ///< [in] ���U���g��
	const double   zeta			  ///< [in] �����萔
	)
  : sampling_rate_(sampleing_rate),
    wn_(wn),
	zeta_(zeta),
	pos_(0.0),
	pos_diff_(0.0),
	vel_(0.0),
	vel_diff_(0.0),
	amplitude_(0.0),
	is_pos_increasing_(false),
	time_(0)
{
}


//=============================================================================
// Oscilator::Constructor
//=============================================================================

/**
 * �f�X�g���N�^�B�قډ������Ȃ��B
 */
Oscilator::~Oscilator()
{
}


//=============================================================================
// Oscilator::Constructor
//=============================================================================

int count = 0;

/**
 * �O�͂���͂���
 */
void Oscilator::InputExternalForce(
	const double force			///< [in] �O��
	)
{
	double a = force
		- 2.0 * this->zeta_ * this->wn_ * this->vel_
		- this->wn_ * this->wn_ * this->pos_;

	static double prev_a = 0.0;

	double new_vel = 0.0;
	{
		double Y = a / this->sampling_rate_ - this->vel_diff_;
		new_vel = this->vel_ + Y;
		this->vel_diff_ = 0;//new_vel - this->vel_ - Y;
	}
	prev_a = a;

	double new_pos = 0.0;
	{
		double Y = new_vel / this->sampling_rate_  - this->pos_diff_;
		new_pos = this->pos_ + Y;
		this->pos_diff_ = 0;//new_pos - this->pos_ - Y;
	}

	if (this->is_pos_increasing_) {
		if (new_pos < this->pos_) {
			printf("start decreasing count %d freq %f\n", count, (double)this->sampling_rate_ / (double)count);
			count++;
			this->is_pos_increasing_ = false;
			if (0.0 < new_pos) {
				this->amplitude_ = std::max(new_pos, this->pos_);
			}
		}
		else {
			if (this->amplitude_ < new_pos) {
				this->amplitude_ = new_pos;
			}
		}
	}
	else {
		if (this->pos_ < new_pos) {
			this->is_pos_increasing_ = true;
			if (new_pos < 0.0) {
				this->amplitude_ = -std::min(new_pos, this->pos_);
			}
		}
		else {
			if (this->amplitude_ < -new_pos) {
				this->amplitude_ = -new_pos;
			}
		}
	}

	printf("f : %e a : %e vel : %e pos : %e : amp : %e\n", force, a, new_vel, new_pos, this->amplitude_);
	//printf("count %d\n", count);

	this->pos_ = new_pos;
	this->vel_ = new_vel;

	if (100.0 < this->amplitude_) {
		fprintf(stderr, "out of range\n");
		exit(-1);
	}
}


//=============================================================================
// Oscilator::Constructor
//=============================================================================

/**
 * ���݈ʒu���擾����
 * 
 * @retval ���݈ʒu
 */
double Oscilator::GetPos()
{
	return this->pos_;
}


//=============================================================================
// Oscilator::GetAmplitude
//=============================================================================

/**
 * ���ݐU�����擾����
 *
 * @retval ���ݐU��
 */
double Oscilator::GetAmplitude()
{
	return this->amplitude_;
}


//=============================================================================
// Oscilator::GetTime
//=============================================================================

/**
 * ���ݎ������擾����
 *
 * @retval ���ݎ���
 */
uint64_t Oscilator::GetTime()
{
	return this->time_;
}


//=============================================================================
// Oscilator::Clear
//=============================================================================

/**
 * ���܂܂ł̌v�Z���ʂ��N���A����
 */
void Oscilator::Clear()
{
	this->pos_ = 0.0;
	this->vel_ = 0.0;
	this->amplitude_ = 0.0;
	this->time_ = 0;
}
