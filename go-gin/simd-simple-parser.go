package main

import (
	"fmt"
	"log"

	"github.com/minio/simdjson-go"
)

// NOTE bad code with no error handling
// just enough for benchmarks

func ParseJsonReqBody(jsonData []byte) (*BatchOpRequest, error) {
	parsedData, err := simdjson.Parse(jsonData, nil)
	if err != nil {
		log.Fatal(err)
	}

	var batchOpReq BatchOpRequest
	_ = parsedData.ForEach(func(batchOp simdjson.Iter) error {
		var elem *simdjson.Element
		elem, err := batchOp.FindElement(elem, "operations")
		if err != nil {
			fmt.Printf("Error %v\n", err)
		} else {
			var opsArray *simdjson.Array
			opsArray, err := elem.Iter.Array(opsArray)
			if err != nil {
				fmt.Printf("Error in getting ops array %v\n", err)
				return err
			}

			parseBatchOpRequest(&batchOpReq, opsArray)
		}

		return nil
	})

	return &batchOpReq, nil
}

func parseBatchOpRequest(req *BatchOpRequest, opsArray *simdjson.Array) error {
	ops := make([]BatchSubOp, 0)
	req.Operations = &ops

	opsArray.ForEach(func(op simdjson.Iter) {

		var subOp BatchSubOp
		// fmt.Printf("\n-----\n")

		var methodElem *simdjson.Element
		methodElem, err := op.FindElement(methodElem, "method")
		if err != nil {
			fmt.Printf("Error Reading Method %v\n", err)
		} else {
			value, _ := methodElem.Iter.String()
			// fmt.Printf("===> Method Found %s \n", value)
			subOp.Method = &value
		}

		var rurlElem *simdjson.Element
		rurlElem, err = op.FindElement(rurlElem, "relative-url")
		if err != nil {
			fmt.Printf("Error Reading Relative URL  %v\n", err)
		} else {
			value, _ := rurlElem.Iter.String()
			// fmt.Printf("===> URL Found %s \n", value)
			subOp.RelativeURL = &value
		}

		var bodyElem *simdjson.Element
		var body PKReadBody
		bodyElem, err = op.FindElement(bodyElem, "body")
		if err != nil {
			fmt.Printf("Error reading body %v\n", err)
		} else {

			var filtersElem *simdjson.Element
			filtersElem, err = bodyElem.Iter.FindElement(filtersElem, "filters")
			if err != nil {
				fmt.Printf("Error Reading Filters  %v\n", err)
			} else {

				var filtersArray *simdjson.Array
				filtersArray, err := filtersElem.Iter.Array(filtersArray)
				if err != nil {
					fmt.Printf("Error in filters array %v\n", err)
				} else {
					filters, _ := parseFilters(req, filtersArray)
					body.Filters = filters
				}

			}

			var readColsElem *simdjson.Element
			readColsElem, err = bodyElem.Iter.FindElement(readColsElem, "readColumns")
			if err != nil {
				fmt.Printf("Error Reading Read Columns  %v\n", err)
			} else {
				var readColsArray *simdjson.Array
				readColsArray, err := readColsElem.Iter.Array(readColsArray)
				if err != nil {
					fmt.Printf("Error in read cols array %v\n", err)
				} else {
					readCols, _ := parseReadCols(req, readColsArray)
					body.ReadColumns = readCols
				}
			}

			var opIDElem *simdjson.Element
			opIDElem, err = bodyElem.Iter.FindElement(opIDElem, "operationId")
			if err != nil {
				fmt.Printf("Error reading operation id  %v\n", err)
			} else {
				value, _ := opIDElem.Iter.String()
				// fmt.Printf("===> OpID Found %s \n", value)
				body.OperationID = &value
			}
		}
		subOp.Body = &body

		ops = append(ops, subOp)

	})

	req.Operations = &ops
	return nil

}
func parseFilters(req *BatchOpRequest, opsArray *simdjson.Array) (*[]Filter, error) {
	filters := make([]Filter, 0)
	opsArray.ForEach(func(op simdjson.Iter) {
		var filter Filter
		var colElem *simdjson.Element
		colElem, err := op.FindElement(colElem, "column")
		if err != nil {
			fmt.Printf("Error Reading Col %v\n", err)
		} else {
			value, _ := colElem.Iter.String()
			// fmt.Printf("===> Col Found %s \n", value)
			filter.Column = &value
		}

		var valueElem *simdjson.Element
		valueElem, err = op.FindElement(valueElem, "value")
		if err != nil {
			fmt.Printf("Error Reading value  %v\n", err)
		} else {
			value, _ := valueElem.Iter.String()
			// fmt.Printf("===> Value Found %s \n", value)
			filter.Value = &value
		}

		filters = append(filters, filter)
	})

	return &filters, nil
}

func parseReadCols(req *BatchOpRequest, opsArray *simdjson.Array) (*[]ReadColumn, error) {
	readCols := make([]ReadColumn, 0)
	opsArray.ForEach(func(op simdjson.Iter) {
		var readCol ReadColumn
		var colElem *simdjson.Element
		colElem, err := op.FindElement(colElem, "column")
		if err != nil {
			fmt.Printf("Error Reading Col %v\n", err)
		} else {
			value, _ := colElem.Iter.String()
			// fmt.Printf("===> Col Found %s \n", value)
			readCol.Column = &value
		}

		var dType *simdjson.Element
		dType, err = op.FindElement(dType, "dataReturnType")
		if err != nil {
			fmt.Printf("Error Reading value  %v\n", err)
		} else {
			value, _ := dType.Iter.String()
			// fmt.Printf("===> Ret Type Found %s \n", value)
			readCol.DataReturnType = &value
		}
		readCols = append(readCols, readCol)
	})

	return &readCols, nil
}
