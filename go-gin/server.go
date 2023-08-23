package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"math/rand"
	"runtime"
	"time"

	"github.com/gin-gonic/gin"
)

type URITime struct {
	Time int `json:"time" uri:"time"`
}

const serialize = true

func main() {
	runtime.GOMAXPROCS(16)
	r := gin.New()
	r.GET("/ping", func(c *gin.Context) {
		c.Writer.WriteString("OK")
	})

	r.GET("/ping/delay/:time", func(c *gin.Context) {
		uriParams := URITime{}
		if err := c.ShouldBindUri(&uriParams); err != nil {
			fmt.Printf("Error %v \n", err)
		}

		time.Sleep(time.Duration(uriParams.Time) * time.Microsecond)

		c.Writer.WriteString("OK")
		c.Status(200)
	})

	r.GET("/ping/rand_delay/:time", func(c *gin.Context) {
		uriParams := URITime{}
		if err := c.ShouldBindUri(&uriParams); err != nil {
			fmt.Printf("Error %v \n", err)
		}

		time.Sleep(time.Duration(rand.Intn(uriParams.Time)) * time.Microsecond)

		c.Writer.WriteString("OK")
		c.Status(200)
	})

	r.POST("/dbopjson/:db/:table", func(c *gin.Context) {
		uriParams := PKReadPP{}
		if err := c.ShouldBindUri(&uriParams); err != nil {
			fmt.Printf("Error %v \n", err)
		}

		postParams := BatchOpRequest{}
		if err := c.BindJSON(&postParams); err != nil {
			fmt.Printf("Error %v\n", err)
			c.AbortWithError(500, err)
			return
		}

		if serialize {
			_, err := json.Marshal(postParams)
			if err != nil {
				fmt.Printf("Error %v\n", err)
				c.AbortWithError(500, err)
				return
			}
		}

		c.Writer.WriteString("OK")
		c.Status(200)
	})

	r.POST("/dbopsimd/:db/:table", func(c *gin.Context) {
		uriParams := PKReadPP{}
		if err := c.ShouldBindUri(&uriParams); err != nil {
			fmt.Printf("Error %v \n", err)
		}

		// Parse JSON:
		_, err := ParseJsonReqBody(streamToByte(c.Request.Body))
		if err != nil {
			fmt.Printf("Error %v\n", err)
			c.AbortWithError(500, err)
			return
		}

		// jsonstr, err := json.MarshalIndent(batchOps, " ", " ")
		// if err != nil {
		// fmt.Printf("Error %v\n", err)
		// c.AbortWithError(500, err)
		// return
		// }
		// fmt.Printf("%s\n", jsonstr)

		c.Writer.WriteString("OK")
		c.Status(200)
	})

	r.Run("0.0.0.0:4046") // listen and serve on 0.0.0.0:8080 (for windows "localhost:8080")
}

func streamToByte(stream io.Reader) []byte {
	buf := new(bytes.Buffer)
	buf.ReadFrom(stream)
	return buf.Bytes()
}
