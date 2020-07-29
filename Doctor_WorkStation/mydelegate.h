#ifndef MYDELEGATE_H
#define MYDELEGATE_H
#include <QItemDelegate>
#include <QComboBox>
#include <QSqlQuery>


class MyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
        MyDelegate(QObject *parent = 0);
        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
            const QModelIndex &index) const
        {
            QComboBox *editor = new QComboBox(parent);
            if(index.column() == 2)
            {
                editor->addItem("男");
                editor->addItem("女");
            }else if(index.column() == 3)
            {
                editor->addItem("DEV-001");
                editor->addItem("DEV-002");
                editor->addItem("DEV-003");
                editor->addItem("DEV-004");
                editor->addItem("DEV-005");
                editor->addItem("DEV-006");
                editor->addItem("DEV-007");
                editor->addItem("DEV-008");
                editor->addItem("DEV-009");
                editor->addItem("DEV-010");
            }else if(index.column() == 0)
            {
                QSqlQuery query;
                query.exec("select dev_id,time from zgmf.ecg_sample");
                int dev_id = query.value(0).toInt();
                if(query.next())
                {
                    for(int i=1;i<=dev_id;i++)
                    {
                           QString namestr=query.value(1).toString();
                           editor->addItem(namestr);
                           query.next();
                    }
                }

            }

            return editor;
        }
        void setEditorData(QWidget *editor, const QModelIndex &index) const;

        void setModelData(QWidget *editor, QAbstractItemModel *model,
            const QModelIndex &index) const;

        void updateEditorGeometry(QWidget *editor,
            const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // MYDELEGATE_H
