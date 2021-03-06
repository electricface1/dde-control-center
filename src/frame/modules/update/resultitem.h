/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DCC_UPDATE_RESULTITEM_H
#define DCC_UPDATE_RESULTITEM_H

#include "widgets/settingsitem.h"

#include "dimagebutton.h"

namespace dcc {
namespace widgets {
class NormalLabel;
}
}

namespace dcc {
namespace update {

class ResultItem : public dcc::widgets::SettingsItem
{
    Q_OBJECT

public:
    explicit ResultItem(QFrame* parent = 0);

    void setSuccess(bool success);
    void setMessage(const QString &message);

private:
    dcc::widgets::NormalLabel* m_message;
    QLabel *m_icon;
};

} // namespace update
} // namespace dcc

#endif // DCC_UPDATE_RESULTITEM_H
