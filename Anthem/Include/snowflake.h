/*
    Copyright (C) 2019 - 2020 Joshua Wade
    Copyright (C) 2019 Henna Haahti
    Original file by shenggan (https://github.com/Shenggan/), licensed under MIT.

    This file is part of Anthem.

    Anthem is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    Anthem is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Anthem. If not, see
                        <https://www.gnu.org/licenses/>.



    Original license below:
    --

    MIT License

    Copyright (c) 2017 shenggan

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H

// https://github.com/Shenggan/SnowFlake/blob/master/SnowFlake.h
// lightlog.h has been replaced by throw.
// gettimeofday() and friends have been replaced by Qt equivalents.
// uint64_t has been replaced by quint64.
#include <stdint.h>
#include <QDateTime>
#include <stdexcept>
#include <mutex>

class SnowFlake {
private:
    static const quint64 start_stmp_ = 1480166465631;
    static const quint64 sequence_bit_ = 12;
    static const quint64 machine_bit_ = 5;
    static const quint64 datacenter_bit_ = 5;

    static const quint64 max_sequence_num_ = -1 ^ (uint64_t(-1) << sequence_bit_);

    static const quint64 machine_left = sequence_bit_;
    static const quint64 datacenter_left = sequence_bit_ + machine_bit_;
    static const quint64 timestmp_left = sequence_bit_ + machine_bit_ + datacenter_bit_;

    quint64 datacenterId;
    quint64 machineId;
    quint64 sequence;
    quint64 lastStmp;

    std::mutex mutex_;

    quint64 getNextMill() {
        quint64 mill = getNewstmp();
        while (mill <= lastStmp) {
            mill = getNewstmp();
        }
        return mill;
    }

    quint64 getNewstmp() {
        QDateTime now = QDateTime::currentDateTimeUtc();
        return quint64(now.toMSecsSinceEpoch());
    }

public:
    static const quint64 max_datacenter_num_ = -1 ^ (uint64_t(-1) << datacenter_bit_);
    static const quint64 max_machine_num_ = -1 ^ (uint64_t(-1) << machine_bit_);

    SnowFlake(int datacenter_Id, int machine_Id) {
        if ((quint64)datacenter_Id > max_datacenter_num_ || datacenter_Id < 0) {
            throw "datacenterId can't be greater than max_datacenter_num_ or less than 0";
        }
        if ((quint64)machine_Id > max_machine_num_ || machine_Id < 0) {
            throw "machineId can't be greater than max_machine_num_or less than 0";
        }
        datacenterId = datacenter_Id;
        machineId = machine_Id;
        sequence = 0L;
        lastStmp = 0L;
    }

    quint64 nextId() {
        std::unique_lock<std::mutex> lock(mutex_);
        quint64 currStmp = getNewstmp();
        if (currStmp < lastStmp) {
            throw "Clock moved backwards.  Refusing to generate id";
        }

        if (currStmp == lastStmp) {
            sequence = (sequence + 1) & max_sequence_num_;
            if (sequence == 0) {
                currStmp = getNextMill();
            }
        } else {
            sequence = 0;
        }
        lastStmp = currStmp;
        return (currStmp - start_stmp_) << timestmp_left
                | datacenterId << datacenter_left
                | machineId << machine_left
                | sequence;
    }

};
#endif // SNOWFLAKE_H
