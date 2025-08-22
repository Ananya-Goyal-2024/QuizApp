#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>

struct Question {
    QString question;
    QString options[4];
    int correctOptionIndex;
};

class QuizApp : public QWidget {
    Q_OBJECT

private:
    QLabel *questionLabel;
    QRadioButton *options[4];
    QButtonGroup *optionGroup;
    QPushButton *nextButton, *submitButton;

    QVector<Question> questions;
    int currentQuestionIndex = 0;
    int score = 0;
    QString userName;

public:
    QuizApp(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Quiz Application");
        resize(600, 300);

        // Ask for user name
        bool ok;
        userName = QInputDialog::getText(this, "User Name", "Enter your name:", QLineEdit::Normal, "", &ok);
        if (!ok || userName.trimmed().isEmpty()) {
            userName = "Anonymous";
        }

        // Load questions
        loadQuestions();

        // Question label
        questionLabel = new QLabel(this);
        questionLabel->setAlignment(Qt::AlignCenter);
        QFont font("Arial", 14, QFont::Bold);
        questionLabel->setFont(font);

        // Options
        optionGroup = new QButtonGroup(this);
        QVBoxLayout *optionLayout = new QVBoxLayout;
        for (int i = 0; i < 4; i++) {
            options[i] = new QRadioButton(this);
            optionGroup->addButton(options[i], i);
            optionLayout->addWidget(options[i]);
        }

        // Buttons
        nextButton = new QPushButton("Next", this);
        submitButton = new QPushButton("Submit", this);
        submitButton->setEnabled(false);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(nextButton);
        buttonLayout->addWidget(submitButton);

        // Main Layout
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(questionLabel);
        mainLayout->addLayout(optionLayout);
        mainLayout->addLayout(buttonLayout);
        setLayout(mainLayout);

        // Connect signals
        connect(nextButton, &QPushButton::clicked, this, &QuizApp::onNextClicked);
        connect(submitButton, &QPushButton::clicked, this, &QuizApp::onSubmitClicked);

        // Display first question
        displayQuestion(currentQuestionIndex);
    }

private slots:
    void onNextClicked() {
        if (!isOptionSelected()) {
            QMessageBox::warning(this, "Warning", "Please select an answer!");
            return;
        }

        if (isAnswerCorrect(currentQuestionIndex)) {
            score++;
        }

        currentQuestionIndex++;
        if (currentQuestionIndex < questions.size()) {
            displayQuestion(currentQuestionIndex);
        } else {
            nextButton->setEnabled(false);
            submitButton->setEnabled(true);
            QMessageBox::information(this, "Info", "Click Submit to finish.");
        }
    }

    void onSubmitClicked() {
        QMessageBox::information(this, "Result",
                                 "Quiz Over! " + userName +
                                 ", Your Score: " + QString::number(score) +
                                 "/" + QString::number(questions.size()));
        saveScore();
        qApp->quit();
    }

private:
    bool isOptionSelected() {
        return optionGroup->checkedButton() != nullptr;
    }

    bool isAnswerCorrect(int index) {
        int correct = questions[index].correctOptionIndex;
        return optionGroup->checkedId() == correct;
    }

    void displayQuestion(int index) {
        optionGroup->setExclusive(false);  // clear selection
        for (int i = 0; i < 4; i++) options[i]->setChecked(false);
        optionGroup->setExclusive(true);

        Question q = questions[index];
        questionLabel->setText("Q" + QString::number(index + 1) + ": " + q.question);
        for (int i = 0; i < 4; i++) {
            options[i]->setText(q.options[i]);
        }
    }

    void loadQuestions() {
        questions.push_back({"What is the capital of India?",
                             {"Delhi", "Mumbai", "Kolkata", "Chennai"}, 0});
        questions.push_back({"Java runs on which platform?",
                             {"Browser", "JVM", "Microcontroller", "BIOS"}, 1});
        questions.push_back({"Which one is not an OOP concept?",
                             {"Inheritance", "Encapsulation", "Polymorphism", "Compilation"}, 3});
        questions.push_back({"Which company developed Java?",
                             {"Google", "Microsoft", "Sun Microsystems", "IBM"}, 2});
        questions.push_back({"Which keyword is used to inherit a class?",
                             {"this", "super", "extends", "implements"}, 2});
    }

    void saveScore() {
        QFile file("scores.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << userName << " - " << score << "/" << questions.size() << "\n";
            file.close();
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QuizApp quiz;
    quiz.show();
    return app.exec();
}

#include "QuizApp_project.moc"

