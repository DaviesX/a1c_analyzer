#include "medicationorder.h"


static int uuid = 0;

csv::MedicationOrder::MedicationOrder(int pid,
                                      bool diabetes,
                                      bool heart_failure,
                                      const std::string& visit_type,
                                      unsigned order_year,
                                      int start_date,
                                      const std::string& order_type,
                                      const std::string& order_status,
                                      const std::string& discontinue_reason,
                                      const std::string& med_category,
                                      const std::string& med_name,
                                      const std::string& order_name,
                                      float dose,
                                      const std::string& uom,
                                      unsigned quantity,
                                      unsigned duration,
                                      unsigned num_refills,
                                      const std::string& route,
                                      const std::string& presc_type,
                                      const std::string& frequency,
                                      bool is_prn,
                                      const std::string& instructions):
        pid(pid),
        diabetes(diabetes),
        heart_failure(heart_failure),
        visit_type(visit_type),
        order_year(order_year),
        start_date(start_date),
        order_type(order_type),
        order_status(order_status),
        discontinue_reason(discontinue_reason),
        med_category(med_category),
        med_name(med_name),
        order_name(order_name),
        dose(dose),
        uom(uom),
        quantity(quantity),
        duration(duration),
        num_refills(num_refills),
        route(route),
        presc_type(presc_type),
        frequency(frequency),
        is_prn(is_prn),
        instructions(instructions)
{
        oid = uuid ++;
}

csv::MedicationOrder::MedicationOrder(int pid):
        pid(pid)
{
}

csv::MedicationOrder::MedicationOrder():
        pid(-1)
{
}

int
csv::MedicationOrder::date() const
{
        return start_date;
}

int
csv::MedicationOrder::end_date() const
{
        return start_date + static_cast<int>(duration);
}
