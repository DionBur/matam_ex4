#include "grades.h"

/*Implementing said stuck in grades.h*/


struct grade_node{
    const char *name;
    int grade;
};

struct grade_node* grade_node_create(const char *name, int id, int grade){
    struct grade_node *grade_node;
    grade_node = malloc(sizeof(struct grade_node));
    if(!grade_node){
        return NULL;
    }
    grade_node->name = name;
    grade_node->grade = grade;
    return grade_node;
}

int grade_node_clone(struct grade_node *original,struct grade_node **clone){
    if(!original || !*clone){
        //one of them is pointing to null
        return 1;
    }
    (*clone)->name = original->name;
    (*clone)->grade = original->grade;
    return 0;
}

void grade_node_destroy(struct grade_node *node){
    if(node){
        free(node);
    }
}
struct student{
    const char *student_name;
    int id;
    struct list *student_grades_list;
};

void student_destroy(struct student *student){
    if(student){
        list_destroy(student->student_grades_list);
        free(student);
    }
}
/*grades is a linked list where each node is a student, and a each student is a struct that contains a name, and idea and a pointer to this students grades list*/
struct grades{
    struct list *student_list;
    element_clone_t grade_node_clone;
    element_destroy_t grade_node_destroy;
};





struct grades* grades_init(){
    struct grades *grades;
    struct list *list;
    grades = malloc(sizeof(struct grades));
    if(!grades){
        return NULL;
    }
    list = list_init((element_clone_t) grade_node_clone, (element_destroy_t) grade_node_destroy);
    if(!list){
        free(grades);
        return NULL;
    }
    grades->student_list = list;
    grades->grade_node_clone = grade_node_clone;
    grades->grade_node_destroy = grade_node_destroy;
    return grades;
}

//check this
void grades_destroy(struct grades *grades){
    struct iterator* it = list_begin(grades);
    struct iterator* tmp;
    while(it){
        tmp = it;
        it = list_next(it);
        student_destroy(tmp);
    }
    free(grades);
}

int grades_add_student(struct grades *grades, const char *name, int id){
    struct student *student;
    student = malloc(sizeof(struct student));
    if(!student){
        return 1;
    }
    student->student_name = name;
    student->id = id;
    student->student_grades_list = list_init(grades->grade_node_clone, grades->grade_node_destroy);
    if(!student->student_grades_list){
        free(student);
        return 1;
    }
    return 0;
}

int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade){
    if(!grades || (grade < 0 || grade > 100)){
        return 1;
    }
    struct iterator* student_it = list_begin(grades);
    struct student* student;
    while(student_it){
        student = list_get(student_it);
        if(student->id == id){ 
            //we found the student
            struct iterator *course_it = list_begin(student->student_grades_list);
            struct grade_node *course;
            while(course_it){
                course = list_get(course_it);
                if(!strcmp(course->name, name)){
                    //the course is already in the list, error
                    return 1;
                }
                course_it = list_next(course_it); 
            }
            course_it = list_end(student->student_grades_list); //paranoia
            struct grade_node *new_grade = grade_node_create(name, id, grade);
            if(!new_grade){
                free(new_grade);
                return 1;
            }
            if(list_insert(course_it, course_it,new_grade)){
                //fails if the element isnt inserted well;
                free(new_grade);
                return 1;
            }
            return 0;
        }
        student_it = list_next(student_it);
    }
    if(!student_it){
        return 1;
    }
}

float grades_calc_avg(struct grades *grades, int id, char **out){
    if(!grades){
        return 1;
    }
    struct iterator *student_it = list_begin(grades);
    struct student *student;
    while(student_it){
        student = list_get(student_it);
        if(student->id == id){
            //we found the student
            *out = malloc(sizeof(char)*(strlen(student->student_name)+1));
            if(!*out){
                return 1;
            }
            strcpy(*out, student->student_name);
            struct iterator *course_it = list_begin(student->student_grades_list);
            struct grade_node *course;
            int sum = 0;
            int count = 0;
            while(course_it){
                course = list_get(course_it);
                sum += course->grade;
                count++;
                course_it = list_next(course_it);
            }
            return (float) sum/count;
        }
    student_it = list_next(student_it);
    }
    if(!student_it){
        return 1;
    }
}

int grades_print_student(struct grades *grades, int id){
    if(!grades){
        return 1;
    }
    struct iterator *student_it = list_begin(grades);
    struct student *student;
    while(student_it){
        student = list_get(student_it);
        if(student->id == id){
            //we found the student
            struct iterator *course_it = list_begin(student->student_grades_list);
            struct grade_node *course;
            course = list_get(course_it);
            printf("%s %d: ", student->student_name, student->id);
            while(course_it){
                course = list_get(course_it);
                if (list_next(course_it) != NULL){ 
                    // Check if there's a next course
                    printf("%s %d, ", course->name, course->grade);
                } 
                else{
                    printf("%s %d", course->name, course->grade); 
                    // No comma for the last course
                }
                course_it = list_next(course_it);
            }
            return 0;
        }
    student_it = list_next(student_it);
    }
    if(!student_it){
        return 1;
    }
}

int grades_print_all(struct grades *grades){
    if(!grades){
        return 1;
    }
    struct iterator *student_it = list_begin(grades);
    struct iterator *course_it;
    struct student *student;
    struct grade_node *course;
    while(student_it){
        student = list_get(student_it);
        course_it = list_begin(student->student_grades_list);
        course = list_get(course_it);
        printf("%s %d: ", student->student_name, student->id);
            while(course_it){
                course = list_get(course_it);
                if (list_next(course_it) != NULL){ 
                    // Check if there's a next course
                    printf("%s %d, ", course->name, course->grade);
                } 
                else{
                    printf("%s %d", course->name, course->grade); 
                    // No comma for the last course
                }
                course_it = list_next(course_it);
            }
        student_it = list_next(student_it);
    }
    if(!student_it){
        return 1;
    }
}