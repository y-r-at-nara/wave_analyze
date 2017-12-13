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
 * コンストラクタ。いったん設定したパラメータは変更不可。
 */
Oscilator::Oscilator(
	const uint32_t sampleing_rate,///< [in] サンプリング周波数
	const double   wn,			  ///< [in] 共振周波数
	const double   zeta			  ///< [in] 減衰定数
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
 * デストラクタ。ほぼ何もしない。
 */
Oscilator::~Oscilator()
{
}


//=============================================================================
// Oscilator::Constructor
//=============================================================================

int count = 0;

/**
 * 外力を入力する
 */
void Oscilator::InputExternalForce(
	const double force			///< [in] 外力
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
 * 現在位置を取得する
 * 
 * @retval 現在位置
 */
double Oscilator::GetPos()
{
	return this->pos_;
}


//=============================================================================
// Oscilator::GetAmplitude
//=============================================================================

/**
 * 現在振幅を取得する
 *
 * @retval 現在振幅
 */
double Oscilator::GetAmplitude()
{
	return this->amplitude_;
}


//=============================================================================
// Oscilator::GetTime
//=============================================================================

/**
 * 現在時刻を取得する
 *
 * @retval 現在時刻
 */
uint64_t Oscilator::GetTime()
{
	return this->time_;
}


//=============================================================================
// Oscilator::Clear
//=============================================================================

/**
 * いままでの計算結果をクリアする
 */
void Oscilator::Clear()
{
	this->pos_ = 0.0;
	this->vel_ = 0.0;
	this->amplitude_ = 0.0;
	this->time_ = 0;
}
