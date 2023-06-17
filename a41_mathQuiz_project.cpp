#include <iostream>
using namespace std;

enum enQuizLevel {Easy =1, MED =2, HARD =3, MIX =4};
enum enOperationType { ADD = 1, SUBTRACT = 2, MULTIPLY = 3, DIVIDE =4, mix =5};
enum enUserAnswer {RIGHT =1, WRONG =2};

struct strOperandRange {
    int number1{};
    int number2{};
};

struct strQuestion {
    strOperandRange operands;
    string sign {""};
    int answer{};
    enOperationType opType;
    enQuizLevel quizLevel;
    enUserAnswer userAnswer{};
    bool isRightAnswer{};
};

struct strQuizInfo {
    // container of questions
    strQuestion questionList[100];
    int numberOFQuestions{};
    enQuizLevel quizLevel;
    enOperationType operationType;
    int numberOfRightAnswers{};
    int numberOfWrongAnswers{};
    // is passed or not
    bool isPassed{};
};

int readQuestionsNumber(string message, string warning) {
    int number{};
    do {
        cout << endl << message << "? ";
        cin >> number;

        if (number <= 0)
            cout << "\a\n" << warning << endl;

    } while (number <= 0);
    return number;
}

int readRange(int from, int to, string message, string warning) {
    int number{};
    do {
        cout << message << "? ";
        cin >> number;

        if (number < from || number > to)
            cout << "\n" << warning << '\n';
    } while (number < from || number > to);
    return number;
}

enQuizLevel getQuizLevel(int quizLEvel) {
    return (enQuizLevel)quizLEvel;
}

enOperationType getOperationType(int operationType) {
    return (enOperationType)operationType;
}

int getRandomNumber(int from, int to) {
    return rand() % (to - from + 1) + from;
}

int add(int number1, int number2) {
    return number1 + number2;
}

int subtract(int number1, int number2) {
    return number1 - number2;
}

int multiply(int number1, int number2) {
    return number1 * number2;
}

int divide(int number1, int number2) {
    return number1 / number2;
}

strOperandRange setOperandRange(enQuizLevel quizLevel) {
    strOperandRange operandRange;

    // mix
    // choose level randomly
    if (quizLevel == enQuizLevel::MIX)
        quizLevel = (enQuizLevel)getRandomNumber(1, 3);

    switch (quizLevel) {
    case enQuizLevel::Easy:
        operandRange.number1 = getRandomNumber(1, 20);
        operandRange.number2 = getRandomNumber(1, 20);
        break;
    case enQuizLevel::MED:
        operandRange.number1 = getRandomNumber(20, 40);
        operandRange.number2 = getRandomNumber(20, 40);
        break;
    default:
        operandRange.number1 = getRandomNumber(40, 120);
        operandRange.number2 = getRandomNumber(40, 120);
    }
    return operandRange;
}

strQuestion getQuestionInfo(enQuizLevel quizLevel, enOperationType operationType) {

    strOperandRange operands = setOperandRange(quizLevel);

    strQuestion question;
    question.operands = operands;

    if (operationType == enOperationType::mix)
        operationType = (enOperationType)getRandomNumber(1, 4);

    switch (operationType) {
    case enOperationType::ADD:
    {
        question.answer = add(operands.number1, operands.number2);
        question.sign = "+";
    }
         break;
    case enOperationType::SUBTRACT:
    {
        question.answer = subtract(operands.number1, operands.number2);
        question.sign = "-";
    }
        break;
    case enOperationType::MULTIPLY:
    {
        question.answer = multiply(operands.number1, operands.number2);
        question.sign = "*";
    }
        break;
    default:
    {
        question.answer = (operands.number2 == 0) ? 0 : divide(operands.number1, operands.number2);
        question.sign = "/";
    }
    }

    return question;
}

string repearString(string str, int numOfRepeatition) {
    string key{ "" };
    for (int number = 0; number < numOfRepeatition; number++)
        key += str;
    return key;
}

enUserAnswer validateUserAnswer(strQuestion question, int userAnswer) {
    return (question.answer == userAnswer) ? enUserAnswer::RIGHT : enUserAnswer::WRONG;
}

string messageUser(enUserAnswer userAnswer) {
    return (userAnswer == enUserAnswer::RIGHT) ? "Right answer :-)" : "Wrong answer :-(";
}

void showQuestionHead(strQuestion question, int cnt, int numberOfQuestions) {
    cout << "\nQuestion [" << cnt + 1 << "/" << numberOfQuestions << "]\n";
    cout << question.operands.number1 << '\n';
    cout << "    " << question.sign << endl;
    cout << question.operands.number2 << '\n';
    cout << repearString("-", 20) << '\n';
}

void showQuestionTail(strQuizInfo& quizInfo,strQuestion question) {
    int userAnswer{};
    cin >> userAnswer;

    enUserAnswer userAns = validateUserAnswer(question, userAnswer);
    cout << messageUser(userAns) << '\n';

    if (userAns == enUserAnswer::WRONG) {
        cout << "the right answer is " << question.answer << endl;
        quizInfo.numberOfWrongAnswers++;
    }
    else
        quizInfo.numberOfRightAnswers++;
}

void showQuestion(strQuizInfo& quizInfo, strQuestion question, int cnt, int numberOfQuestions) {
    showQuestionHead(question,cnt,numberOfQuestions);
    showQuestionTail(quizInfo, question);
}

void quizHead(strQuizInfo& quizInfo) {
    int numberOfQuestions = readQuestionsNumber("How many number of questions do you want", "Invalid input");

    string message = "Enter question level [1] Easy, [2] Med, [3] Hard, [4] Mix";
    enQuizLevel quizLevel = getQuizLevel(readRange(1,4,message,"Invalid range"));

    message = "Enter operation type [1] Add, [2] Subtract, [3] Multiply, [4] Divide, [5] Mix";
    enOperationType operationType = getOperationType(readRange(1, 5, message, "Invalid range"));

    quizInfo.numberOFQuestions = numberOfQuestions;
    quizInfo.quizLevel = quizLevel;
    quizInfo.operationType = operationType;
}

void quizTail(strQuizInfo& quizInfo) {
    int numberOfQuestions = quizInfo.numberOFQuestions;
    strQuestion question;
   
    for (int number = 0; number < numberOfQuestions; number++) {
        question = getQuestionInfo(quizInfo.quizLevel, quizInfo.operationType);
        showQuestion(quizInfo, question, number, numberOfQuestions);

    }
}

string questionLevel(enQuizLevel quizLevel) {
    string levels[] = {"Easy","Medium","Hard","Mix"};
    return levels[quizLevel - 1];
}

string operType(enOperationType operType) {
    string types[] = {"Add", "Subtract", "Multiply", "Divide", "Mix"};
    return types[operType - 1];
}

void quizSummary(strQuizInfo quizInfo) {
    cout << "number of questions " << quizInfo.numberOFQuestions << endl;
    cout << "question level " << questionLevel(quizInfo.quizLevel) << endl;
    cout << "operation type " << operType(quizInfo.operationType) << endl;
    cout << "number of right questions " << quizInfo.numberOfRightAnswers << endl;
    cout << "number of wrong questions " << quizInfo.numberOfWrongAnswers << endl;
}

string finalResult(strQuizInfo quizInfo) {
    if (quizInfo.numberOfRightAnswers > quizInfo.numberOfWrongAnswers)
        return "final result is passed :-)";
    else if (quizInfo.numberOfRightAnswers < quizInfo.numberOfWrongAnswers)
        return "final result is failed :-(";
    else
        return "final result is draw -_-";
}

string repeatString(string str, int numOfRepeatition) {
    string key{ "" };
    for (int number = 0; number < numOfRepeatition; number++)
        key += str;
    return key;
}

void finalResultScreen(string finalRes) {
    cout << repeatString("_", 30) << endl;
    cout << finalRes << endl;
    cout << repeatString("_", 30) << endl;
}

void startQuiz(strQuizInfo quizInfo) {
    quizHead(quizInfo);
    quizTail(quizInfo);
    finalResultScreen(finalResult(quizInfo));
    quizSummary(quizInfo);
}

void cleanScreen() {
    system("cls");
    system("color 0f");
}

strQuizInfo resetData() {
    strQuizInfo quizInfo;
    quizInfo.numberOFQuestions = 0;
    quizInfo.numberOfRightAnswers = 0;
    quizInfo.numberOfWrongAnswers = 0;
    return quizInfo;
}

void run() {
    char ans;
    do {
        startQuiz(resetData());
        cout << "\nDo you want to play again? (y-n): ";
        cin >> ans;
        if (ans == 'Y' || ans == 'y') {
            cleanScreen();
        }

            // remove quizInfo
    } while (ans == 'Y' || ans == 'y');
}

int main()
{
    srand((unsigned)time(NULL));
    run();
    

    return 0;
}
