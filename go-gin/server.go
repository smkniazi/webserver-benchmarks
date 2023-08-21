package main

import (
	"fmt"
	"math/rand"
	"runtime"
	"time"

	"github.com/gin-gonic/gin"
)

type URITime struct {
	Time int `json:"time" uri:"time"`
}

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

	r.POST("/dbop/:db/:table", func(c *gin.Context) {
		uriParams := PKReadPP{}
		if err := c.ShouldBindUri(&uriParams); err != nil {
			fmt.Printf("Error %v \n", err)
		}

		postParams := BatchOpRequest{}
		if err := c.BindJSON(&postParams); err != nil {
			fmt.Printf("Error %v\n", err)
			c.AbortWithError(400, err)
			return
		}

		// b, err := json.MarshalIndent(postParams, "", " ")
		// if err != nil {
		// fmt.Printf("Error %v\n", err)
		// c.AbortWithError(400, err)
		// return
		// }
		// fmt.Printf("Req is %s \n", string(b))

		c.Writer.WriteString("OK")
		c.Status(200)
	})

	r.Run("0.0.0.0:4046") // listen and serve on 0.0.0.0:8080 (for windows "localhost:8080")
}
