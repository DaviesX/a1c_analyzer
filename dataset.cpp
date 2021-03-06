#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <sstream>
#include <functional>
#include "labmeasure.h"
#include "medicationorder.h"
#include "dataset.h"


void
dataset::sort(std::vector<csv::LabMeasure>& a)
{
        std::sort(a.begin(), a.end(), [](const csv::LabMeasure& x, const csv::LabMeasure& y) {
                return x.date() < y.date();
        });
}

void
dataset::sort(std::vector<csv::MedicationOrder>& a)
{
        std::sort(a.begin(), a.end(), [](const csv::MedicationOrder& x, const csv::MedicationOrder& y) {
                return x.date() < y.date();
        });
}

void
dataset::make(const std::vector<csv::LabMeasure>& in, patient_measures_t& out)
{
        for (const csv::LabMeasure& measure: in) {
                patient_measures_t::iterator it = out.find(measure.pid);
                if (it == out.end()) {
                        std::vector<csv::LabMeasure> measures;
                        measures.push_back(measure);
                        out.insert(patient_measure_t(measure.pid, measures));
                } else {
                        it->second.push_back(measure);
                }
        }
}

void
dataset::make(const std::vector<csv::MedicationOrder>& in,
              const std::set<csv::MedCategory>& med_categ,
              const std::set<csv::Order2Category>& order2categ, patient_orders_t& out)
{
        for (csv::MedicationOrder order: in) {
                auto o2c_it = order2categ.find(order.order_name);
                if (o2c_it != order2categ.end()) {
                        if (o2c_it->med_code != -1)	order.med_category = o2c_it->med_categ;
                        else				continue;
                }

                auto categ_it = med_categ.find(order.med_category);
                if (categ_it != med_categ.end())        order.med_class = categ_it->get_class();
                else 					order.med_class = -1;

                patient_orders_t::iterator it = out.find(order.pid);
                if (it == out.end()) {
                        std::vector<csv::MedicationOrder> orders;
                        orders.push_back(order);
                        out.insert(patient_order_t(order.pid, orders));
                } else {
                        it->second.push_back(order);
                }
        }
}

void
dataset::join(std::set<csv::Order2Category>& order_map, const std::set<csv::MedCategory>& med_categ)
{
        std::map<int, std::string> code2categ;
        for (csv::MedCategory categ: med_categ)
                code2categ.insert(std::pair<int, std::string>(categ.get_code(), categ.get_category()));

        for (std::set<csv::Order2Category>::iterator it = order_map.begin(); it != order_map.end(); ++ it) {
                auto corr = code2categ.find(it->med_code);
                if (corr != code2categ.end()) {
                        it->med_categ = corr->second;
                }
        }
}

static bool substring_in(const std::set<filter::LabFilter>& filter, const std::string& s)
{
        for (filter::LabFilter f: filter) {
                if (s.npos != s.find(f.keep)) {
                        return true;
                }
        }
        return false;
}

void
dataset::filter(const patient_measures_t& measures, const std::set<filter::LabFilter>& filter, patient_measures_t& filtered)
{
        for (patient_measures_t::const_iterator it = measures.begin(); it != measures.end(); ++ it) {
                std::vector<csv::LabMeasure> target;
                const std::vector<csv::LabMeasure>& measures = it->second;
                for (const csv::LabMeasure& measure: measures) {
                        if (::substring_in(filter, measure.observation)) {
                                target.push_back(measure);
                        }
                }
                if (!target.empty())
                        filtered.insert(patient_measure_t(it->first, target));
        }
}

void
dataset::filter(const patient_orders_t& orders, const std::set<filter::DrugFilter>& filter, patient_orders_t& filtered)
{
        for (patient_orders_t::const_iterator it = orders.begin(); it != orders.end(); ++ it) {
                std::vector<csv::MedicationOrder> target;
                const std::vector<csv::MedicationOrder>& orders = it->second;
                for (const csv::MedicationOrder& order: orders) {
                        if (filter.end() == filter.find(filter::DrugFilter(order.med_category))) {
                                target.push_back(order);
                        }
                }
                if (!target.empty())
                        filtered.insert(patient_order_t(it->first, target));
        }
}

void
dataset::join_time_asc(const patient_measures_t& measures, const patient_orders_t& orders, patient_records_t& records)
{
        for (patient_measures_t::const_iterator it = measures.begin(); it != measures.end(); ++ it) {
                analysis::PatientRecord record(it->first);
                record.add_measures(it->second);
                patient_orders_t::const_iterator order_iter = orders.find(it->first);
                if (order_iter != orders.end()) {
                        record.add_orders(order_iter->second);
                        record.sort_all();
                }
                records.insert(patient_record_t(it->first, record));
        }
}

void
dataset::delta(const patient_records_t& record, float a1c, std::vector<csv::Delta>& delta)
{
        for (patient_records_t::const_iterator it = record.begin(); it != record.end(); ++ it) {
                std::vector<csv::Delta> patient_deltas;
                it->second.get_analysis(a1c, patient_deltas);
                delta.insert(delta.end(), patient_deltas.begin(), patient_deltas.end());
        }
}

void
dataset::delta(const patient_records_t& record, float a1c, std::vector<csv::SimpleDelta>& delta)
{
        for (patient_records_t::const_iterator it = record.begin(); it != record.end(); ++ it) {
                std::vector<csv::SimpleDelta> patient_deltas;
                it->second.get_analysis(a1c, patient_deltas);
                delta.insert(delta.end(), patient_deltas.begin(), patient_deltas.end());
        }
}

void
dataset::statistics(const patient_records_t& records, float a1c, analysis::Statistics& stat)
{
        unsigned n = 0;
        unsigned r = 0;
        for (patient_records_t::const_iterator it = records.begin(); it != records.end(); ++ it) {
                bool has_over_margin = it->second.has_over_margin(a1c);
                bool has_recovered = it->second.has_recovered(a1c);
                r += has_over_margin && has_recovered;
                n += has_over_margin;
                if (has_recovered && has_over_margin) {
                        unsigned l = it->second.total_recovery_length(a1c);
                        analysis::int_dist_add_sample(stat.dist_r_len,
                                                      analysis::int_dist_ent_t(static_cast<int>(l), 1));
                }
        }
        analysis::int_dist_normalize(stat.dist_r_len);
        stat.p_recovered = static_cast<float>(r)/n;
}
